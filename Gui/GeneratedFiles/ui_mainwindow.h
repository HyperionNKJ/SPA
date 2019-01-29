/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    CodeEditor *txtCodeEditor;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout;
    QTextEdit *txtQuery;
    QGroupBox *groupBox_3;
    QHBoxLayout *horizontalLayout_3;
    QTextEdit *txtResult;
    QPushButton *btnLoadQuery;
    QPushButton *btnRunQuery;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(852, 751);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        txtCodeEditor = new CodeEditor(groupBox_2);
        txtCodeEditor->setObjectName(QStringLiteral("txtCodeEditor"));

        horizontalLayout_2->addWidget(txtCodeEditor);


        verticalLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 80));
        groupBox->setCheckable(false);
        horizontalLayout = new QHBoxLayout(groupBox);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        txtQuery = new QTextEdit(groupBox);
        txtQuery->setObjectName(QStringLiteral("txtQuery"));
        txtQuery->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);

        horizontalLayout->addWidget(txtQuery);


        verticalLayout->addWidget(groupBox);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setMaximumSize(QSize(16777215, 100));
        horizontalLayout_3 = new QHBoxLayout(groupBox_3);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        txtResult = new QTextEdit(groupBox_3);
        txtResult->setObjectName(QStringLiteral("txtResult"));

        horizontalLayout_3->addWidget(txtResult);


        verticalLayout->addWidget(groupBox_3);

        btnLoadQuery = new QPushButton(centralwidget);
        btnLoadQuery->setObjectName(QStringLiteral("btnLoadQuery"));

        verticalLayout->addWidget(btnLoadQuery);

        btnRunQuery = new QPushButton(centralwidget);
        btnRunQuery->setObjectName(QStringLiteral("btnRunQuery"));

        verticalLayout->addWidget(btnRunQuery);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 852, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(btnLoadQuery, SIGNAL(clicked()), MainWindow, SLOT(btnLoad_clicked()));
        QObject::connect(btnRunQuery, SIGNAL(clicked()), MainWindow, SLOT(btnRun_clicked()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "SPA GUI", nullptr));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "SIMPLE Source Code Editor", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "PQL Query - input one query at a time", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Query Results", nullptr));
        btnLoadQuery->setText(QApplication::translate("MainWindow", "Load SIMPLE source code from file", nullptr));
        btnRunQuery->setText(QApplication::translate("MainWindow", "Evaluate PQL qeury", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
