/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QStackedWidget *stackedWidget;
    QWidget *pageLogin;
    QGroupBox *groupBox;
    QLabel *label_7;
    QLabel *label_8;
    QSpinBox *serPort;
    QLineEdit *lineServer;
    QSpinBox *loaPort;
    QLabel *label_6;
    QGroupBox *groupBox_2;
    QLabel *label;
    QLineEdit *lineUser;
    QLabel *label_2;
    QLineEdit *linepword;
    QPushButton *butLogin;
    QWidget *pageState;
    QPushButton *butHidde;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(480, 273);
        MainWindow->setMinimumSize(QSize(480, 273));
        MainWindow->setMaximumSize(QSize(480, 273));
        QIcon icon;
        icon.addFile(QStringLiteral(":/ico/ed"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        stackedWidget = new QStackedWidget(centralWidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        pageLogin = new QWidget();
        pageLogin->setObjectName(QStringLiteral("pageLogin"));
        groupBox = new QGroupBox(pageLogin);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 451, 81));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(264, 42, 81, 20));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 42, 81, 21));
        serPort = new QSpinBox(groupBox);
        serPort->setObjectName(QStringLiteral("serPort"));
        serPort->setGeometry(QRect(360, 36, 81, 31));
        serPort->setMinimum(1000);
        serPort->setMaximum(10000);
        serPort->setValue(8888);
        lineServer = new QLineEdit(groupBox);
        lineServer->setObjectName(QStringLiteral("lineServer"));
        lineServer->setGeometry(QRect(100, 37, 131, 31));
        loaPort = new QSpinBox(pageLogin);
        loaPort->setObjectName(QStringLiteral("loaPort"));
        loaPort->setGeometry(QRect(360, 223, 81, 31));
        loaPort->setMinimum(1000);
        loaPort->setMaximum(10000);
        loaPort->setValue(6666);
        label_6 = new QLabel(pageLogin);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(254, 230, 101, 16));
        groupBox_2 = new QGroupBox(pageLogin);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 90, 451, 131));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 40, 62, 19));
        lineUser = new QLineEdit(groupBox_2);
        lineUser->setObjectName(QStringLiteral("lineUser"));
        lineUser->setGeometry(QRect(90, 40, 231, 31));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 80, 62, 19));
        linepword = new QLineEdit(groupBox_2);
        linepword->setObjectName(QStringLiteral("linepword"));
        linepword->setGeometry(QRect(90, 80, 231, 31));
        linepword->setEchoMode(QLineEdit::Password);
        butLogin = new QPushButton(groupBox_2);
        butLogin->setObjectName(QStringLiteral("butLogin"));
        butLogin->setGeometry(QRect(340, 40, 101, 71));
        stackedWidget->addWidget(pageLogin);
        pageState = new QWidget();
        pageState->setObjectName(QStringLiteral("pageState"));
        butHidde = new QPushButton(pageState);
        butHidde->setObjectName(QStringLiteral("butHidde"));
        butHidde->setGeometry(QRect(130, 180, 191, 41));
        stackedWidget->addWidget(pageState);

        horizontalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\351\205\215\347\275\256\344\277\241\346\201\257\357\274\232", 0));
        label_7->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\347\253\257\345\217\243\357\274\232", 0));
        label_8->setText(QApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250\345\234\260\345\235\200\357\274\232", 0));
        lineServer->setText(QString());
        label_6->setText(QApplication::translate("MainWindow", "\346\234\254\345\234\260\347\233\221\345\220\254\347\253\257\345\217\243\357\274\232", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\347\231\273\345\275\225\344\277\241\346\201\257\357\274\232", 0));
        label->setText(QApplication::translate("MainWindow", "<html><head/><body><p>\347\224\250\346\210\267\345\220\215\357\274\232</p></body></html>", 0));
        lineUser->setInputMask(QString());
        lineUser->setText(QString());
        label_2->setText(QApplication::translate("MainWindow", "<html><head/><body><p>\345\257\206    \347\240\201\357\274\232</p></body></html>", 0));
        linepword->setText(QString());
        butLogin->setText(QApplication::translate("MainWindow", "\347\231\273\345\275\225", 0));
        butHidde->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217\347\252\227\345\217\243", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
