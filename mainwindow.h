#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QListWidgetItem>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    mainwindow(QWidget *parent = nullptr);
    ~mainwindow();

private slots:
    void onItemChanged(QListWidgetItem *item);
    void onChangeDirectoryClicked();
    void on_addProfileButton_clicked();
    void on_deleteProfileButton_clicked();
    void on_profileComboBox_currentIndexChanged(int index);
    void on_addStartupItemButton_clicked();
    void on_listWidget_itemChanged(QListWidgetItem *item);
    void applyProfile(const QString &profileName);



private:
    Ui::mainwindow *ui;
    bool isHidden(const QString &filePath);
    void toggleStartupItem(const QString &appName, bool enable);
    void loadStartupItems();
    void setHiddenAttribute(const QString &filePath, bool hidden);
    QString currentStartupDir;
    QMap<QString, QMap<QString, bool>> profiles; // Storingg the profiles and a map of their states
    void saveProfiles();
    void loadProfiles();
    void updateProfileComboBox();
    void loadStartupItemsForProfile(const QString& profileName);
    QStringList getCurrentStartupItems();






};
#endif // MAINWINDOW_H
