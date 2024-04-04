/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainwindow
{
public:
    QWidget *centralwidget;
    QPushButton *addStartupItemButton;
    QPushButton *addProfileButton;
    QListWidget *listWidget;
    QPushButton *changeDirButton;
    QComboBox *profileComboBox;
    QPushButton *deleteProfileButton;
    QMenuBar *menubar;
    QMenu *menuHome;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainwindow)
    {
        if (mainwindow->objectName().isEmpty())
            mainwindow->setObjectName(QString::fromUtf8("mainwindow"));
        mainwindow->resize(800, 600);
        centralwidget = new QWidget(mainwindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        addStartupItemButton = new QPushButton(centralwidget);
        addStartupItemButton->setObjectName(QString::fromUtf8("addStartupItemButton"));
        addStartupItemButton->setGeometry(QRect(620, 100, 151, 30));
        addProfileButton = new QPushButton(centralwidget);
        addProfileButton->setObjectName(QString::fromUtf8("addProfileButton"));
        addProfileButton->setGeometry(QRect(630, 310, 131, 30));
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setGeometry(QRect(40, 80, 561, 431));
        changeDirButton = new QPushButton(centralwidget);
        changeDirButton->setObjectName(QString::fromUtf8("changeDirButton"));
        changeDirButton->setGeometry(QRect(355, 30, 191, 30));
        profileComboBox = new QComboBox(centralwidget);
        profileComboBox->addItem(QString());
        profileComboBox->addItem(QString());
        profileComboBox->setObjectName(QString::fromUtf8("profileComboBox"));
        profileComboBox->setGeometry(QRect(630, 250, 131, 29));
        deleteProfileButton = new QPushButton(centralwidget);
        deleteProfileButton->setObjectName(QString::fromUtf8("deleteProfileButton"));
        deleteProfileButton->setGeometry(QRect(630, 380, 131, 30));
        mainwindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainwindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 26));
        menuHome = new QMenu(menubar);
        menuHome->setObjectName(QString::fromUtf8("menuHome"));
        mainwindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainwindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainwindow->setStatusBar(statusbar);

        menubar->addAction(menuHome->menuAction());

        retranslateUi(mainwindow);

        QMetaObject::connectSlotsByName(mainwindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainwindow)
    {
        mainwindow->setWindowTitle(QCoreApplication::translate("mainwindow", "mainwindow", nullptr));
        addStartupItemButton->setText(QCoreApplication::translate("mainwindow", "Add Startup Item", nullptr));
        addProfileButton->setText(QCoreApplication::translate("mainwindow", "Add Profile", nullptr));
        changeDirButton->setText(QCoreApplication::translate("mainwindow", "Change  Directory", nullptr));
        profileComboBox->setItemText(0, QCoreApplication::translate("mainwindow", "config1", nullptr));
        profileComboBox->setItemText(1, QCoreApplication::translate("mainwindow", "config2", nullptr));

        deleteProfileButton->setText(QCoreApplication::translate("mainwindow", "Delete Profile", nullptr));
        menuHome->setTitle(QCoreApplication::translate("mainwindow", "Home", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainwindow: public Ui_mainwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
