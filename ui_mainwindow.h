/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *GrayLabel;
    QLabel *DepthLabel;
    QCheckBox *ShowGray;
    QCheckBox *ShowDepth;
    QPushButton *StartButton;
    QLineEdit *TextOut;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(669, 343);
        MainWindow->setWindowOpacity(1.000000000000000);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        GrayLabel = new QLabel(centralwidget);
        GrayLabel->setObjectName(QString::fromUtf8("GrayLabel"));
        GrayLabel->setGeometry(QRect(10, 10, 320, 240));
        GrayLabel->setPixmap(QPixmap(QString::fromUtf8(":/img/gray.jpg")));
        DepthLabel = new QLabel(centralwidget);
        DepthLabel->setObjectName(QString::fromUtf8("DepthLabel"));
        DepthLabel->setGeometry(QRect(340, 10, 320, 240));
        DepthLabel->setPixmap(QPixmap(QString::fromUtf8(":/img/rgb.jpg")));
        ShowGray = new QCheckBox(centralwidget);
        ShowGray->setObjectName(QString::fromUtf8("ShowGray"));
        ShowGray->setGeometry(QRect(10, 260, 101, 31));
        QFont font;
        font.setPointSize(15);
        ShowGray->setFont(font);
        ShowDepth = new QCheckBox(centralwidget);
        ShowDepth->setObjectName(QString::fromUtf8("ShowDepth"));
        ShowDepth->setGeometry(QRect(110, 260, 101, 31));
        ShowDepth->setFont(font);
        StartButton = new QPushButton(centralwidget);
        StartButton->setObjectName(QString::fromUtf8("StartButton"));
        StartButton->setGeometry(QRect(220, 260, 91, 31));
        StartButton->setFont(font);
        TextOut = new QLineEdit(centralwidget);
        TextOut->setObjectName(QString::fromUtf8("TextOut"));
        TextOut->setGeometry(QRect(320, 260, 341, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 669, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\205\245\344\276\265\346\243\200\346\265\213", nullptr));
        GrayLabel->setText(QString());
        DepthLabel->setText(QString());
        ShowGray->setText(QCoreApplication::translate("MainWindow", "\347\201\260\345\272\246\345\233\276\345\203\217", nullptr));
        ShowDepth->setText(QCoreApplication::translate("MainWindow", "\346\267\261\345\272\246\344\277\241\346\201\257", nullptr));
        StartButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\347\250\213\345\272\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
