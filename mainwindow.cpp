#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QSettings>
#include <QDir>
#include <QFileInfoList>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainwindow)
{
    ui->setupUi(this);

    // Connect ui
    connect(ui->addProfileButton, &QPushButton::clicked,
            this, &mainwindow::on_addProfileButton_clicked);
    connect(ui->deleteProfileButton, &QPushButton::clicked,
            this, &mainwindow::on_deleteProfileButton_clicked);
    connect(ui->profileComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &mainwindow::on_profileComboBox_currentIndexChanged);
    connect(ui->addStartupItemButton, &QPushButton::clicked,
            this, &mainwindow::on_addStartupItemButton_clicked);
    connect(ui->changeDirButton, &QPushButton::clicked,
            this, &mainwindow::onChangeDirectoryClicked);
    connect(ui->listWidget, &QListWidget::itemChanged,
            this, &mainwindow::onItemChanged);
    currentStartupDir = QDir::homePath() + "/.config/autostart/";


    // Initialize  status bar
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage("Ready");

    // Load profiles from settings
    loadProfiles();
    if (profiles.isEmpty()) {
        // No profiles ? ok create a default profile with all items enabled
        QDir autostartDir(QDir::homePath() + "/.config/autostart/");
        QFileInfoList fileList = autostartDir.entryInfoList(QDir::Files);
        QMap<QString, bool> defaultItems;
        for (const QFileInfo &fileInfo : fileList) {
            defaultItems[fileInfo.completeBaseName()] = true; // Check all items by default
        }
        profiles["Default"] = defaultItems;
        saveProfiles();
    }

    // refresh  UI to reflect the loaded/default profile
    updateProfileComboBox();
    ui->profileComboBox->setCurrentIndex(ui->profileComboBox->findText("Default"));
    loadStartupItemsForProfile(ui->profileComboBox->currentText());
}


mainwindow::~mainwindow()
{
    delete ui;
}

void mainwindow::onItemChanged(QListWidgetItem *item) {
    if(item->checkState() == Qt::Checked) {
        statusBar()->showMessage("Startup Item Enabled: " + item->text());
    } else {
        statusBar()->showMessage("Startup Item Disabled: " + item->text());
    }

    QSettings settings;
       if(item->checkState() == Qt::Checked) {
           settings.setValue(item->text(), "enabled");
       } else {
           settings.setValue(item->text(), "disabled");
       }

       QString filePath = QDir::homePath() + "/.config/autostart/" + item->text() + ".desktop";

          if (item->checkState() == Qt::Checked) {
              statusBar()->showMessage("Startup Item Enabled: " + item->text());
              setHiddenAttribute(filePath, false);
          } else {
              statusBar()->showMessage("Startup Item Disabled: " + item->text());
              setHiddenAttribute(filePath, true);
          }
}
bool mainwindow::isHidden(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed() == "Hidden=true") {
                return true;
            }
        }
    }
    return false;
  }

void mainwindow::loadStartupItems() {
    QDir autostartDir(currentStartupDir);
    QFileInfoList fileList = autostartDir.entryInfoList(QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.baseName();
        QListWidgetItem *item = new QListWidgetItem(fileName, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        // cheeeck for hidden
        if (isHidden(fileInfo.absoluteFilePath())) {
            item->setCheckState(Qt::Unchecked);
        } else {
            item->setCheckState(Qt::Checked);
        }
    }
}
void mainwindow::setHiddenAttribute(const QString &filePath, bool hidden) {
    QFile file(filePath);
    QStringList lines;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            lines.append(in.readLine());
        }
        file.close();
    }

    int hiddenIndex = -1;
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].startsWith("Hidden=")) {
            hiddenIndex = i;
            break;
        }
    }

    if (hiddenIndex != -1) {
        lines[hiddenIndex] = QString("Hidden=") + (hidden ? "true" : "false");

    } else {
        lines.append(QString("Hidden=") + (hidden ? "true" : "false"));
    }

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const QString &line : lines) {
            out << line << "\n";
        }
    }
}

void mainwindow::onChangeDirectoryClicked() {
    QString newDir = QFileDialog::getExistingDirectory(this, "Select Startup Directory", currentStartupDir);

    if (!newDir.isEmpty()) {
        currentStartupDir = newDir;
        loadStartupItems();  // load the list of startup stuff
    }
}

void mainwindow::on_addProfileButton_clicked() {
    bool ok;
    QString profileName = QInputDialog::getText(this, tr("Add Profile"),
                                                tr("Profile Name:"), QLineEdit::Normal,
                                                "", &ok);
    if (ok && !profileName.isEmpty()) {
        if (profiles.contains(profileName)) {
            QMessageBox::warning(this, tr("Duplicate Profile"),
                                 tr("A profile with this name already exist"));
            return;
        }

        profiles.insert(profileName, profiles.value("Default")); // Copy from  Default profile
        updateProfileComboBox();
        saveProfiles();
        ui->profileComboBox->setCurrentText(profileName);
        loadStartupItemsForProfile(profileName);
    }
}


void mainwindow::on_deleteProfileButton_clicked() {
    QString profileName = ui->profileComboBox->currentText();
    if (profileName.isEmpty()) {
        return;
    }

    int ret = QMessageBox::warning(this, tr("Delete Profile"),
                                   tr("Are you sure you want to delete this profile?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        profiles.remove(profileName);
        updateProfileComboBox();
        saveProfiles();
        if (!profiles.isEmpty()) {
            ui->profileComboBox->setCurrentIndex(0); // Set first profile current
            loadStartupItemsForProfile(profiles.keys().first()); // Load startup items for first profil
        }
    }
}
QStringList mainwindow::getCurrentStartupItems() {
    QStringList startupItems;
    QDir autostartDir(currentStartupDir);
    QFileInfoList fileList = autostartDir.entryInfoList(QDir::Files);

    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.baseName();
        startupItems.append(fileName);
    }
    return startupItems;
}
void mainwindow::on_addStartupItemButton_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this,
        tr("Select Startup Item"), QDir::homePath(), tr("Startup Files (*.desktop)"));

    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString baseName = fileInfo.completeBaseName();

        // Ensure dest is correct
        QString destination = currentStartupDir + "/" + fileInfo.fileName();

        if (QFile::copy(filePath, destination)) {
            // file copied? ok update profile
            QString currentProfile = ui->profileComboBox->currentText();
            profiles[currentProfile][baseName] = true; // enable item in current profile
            saveProfiles();
            loadStartupItemsForProfile(currentProfile);
        } else {
            // copy failed? ok show an error message
            QMessageBox::warning(this, tr("Error"),
                                 tr("Couldnt add startup item. A file with the same name may exist in the destination"));
        }
    }
}


void mainwindow::on_profileComboBox_currentIndexChanged(int index) {
    if (index != -1) {
        QString profileName = ui->profileComboBox->itemText(index);
        loadStartupItemsForProfile(profileName);
    }
}

// Function to save profiles to QSettings
void mainwindow::saveProfiles() {
    QSettings settings("YourCompany", "StartupManager");
    settings.beginWriteArray("profiles");
    int i = 0;
    for (const QString &profileName : profiles.keys()) {
        settings.setArrayIndex(i++);
        settings.setValue("name", profileName);
        settings.beginWriteArray("items");
        int j = 0;
        for (const QString &itemName : profiles[profileName].keys()) {
            settings.setArrayIndex(j++);
            settings.setValue("itemName", itemName);
            settings.setValue("enabled", profiles[profileName][itemName]);
        }
        settings.endArray();
    }
    settings.endArray();
}

// Function to load profiles from QSettings
void mainwindow::loadProfiles() {
    QSettings settings("YourCompany", "StartupManager");
    int size = settings.beginReadArray("profiles");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        QString profileName = settings.value("name").toString();
        int itemsSize = settings.beginReadArray("items");
        QMap<QString, bool> items;
        for (int j = 0; j < itemsSize; ++j) {
            settings.setArrayIndex(j);
            QString itemName = settings.value("itemName").toString();
            bool enabled = settings.value("enabled").toBool();
            items[itemName] = enabled;
        }
        settings.endArray();
        profiles[profileName] = items;
    }
    settings.endArray();
}
void mainwindow::applyProfile(const QString &profileName) {
    QMap<QString, bool> &items = profiles[profileName];
    QDir autostartDir(currentStartupDir);
    QFileInfoList fileList = autostartDir.entryInfoList(QDir::Files);
    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.completeBaseName();
        setHiddenAttribute(fileName, !items.value(fileName, true));
    }
}

// Function to update profile comboo box
void mainwindow::updateProfileComboBox() {
    ui->profileComboBox->clear();
    ui->profileComboBox->addItems(profiles.keys());
}

void mainwindow::loadStartupItemsForProfile(const QString& profileName) {
    QMap<QString, bool> startupItems = profiles.value(profileName);
    ui->listWidget->clear(); // Clear current list

    QDir autostartDir(currentStartupDir);
    QFileInfoList fileList = autostartDir.entryInfoList(QDir::Files);
    for (const QFileInfo &fileInfo : fileList) {
        QString fileName = fileInfo.baseName();
        QListWidgetItem* listItem = new QListWidgetItem(fileName, ui->listWidget);
        listItem->setFlags(listItem->flags() | Qt::ItemIsUserCheckable);

        // Set the check state based on the profile settings
        listItem->setCheckState(startupItems.value(fileName, true) ? Qt::Checked : Qt::Unchecked);
    }
}

void mainwindow::on_listWidget_itemChanged(QListWidgetItem *item) {
    QString currentProfile = ui->profileComboBox->currentText();
    QString itemName = item->text();
    bool enabled = (item->checkState() == Qt::Checked);

    profiles[currentProfile][itemName] = enabled;
    setHiddenAttribute(itemName, !enabled);
    saveProfiles();
}

