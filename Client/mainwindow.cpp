#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSystemTrayIcon>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    trayIcon = new QSystemTrayIcon(QIcon(":/ico/un"),this);
    trayIcon->setToolTip(tr("未连接到服务器！"));
    ser = new TcpServer(this);
    connect(trayIcon,&QSystemTrayIcon::activated,[&](QSystemTrayIcon::ActivationReason active){
        if (QSystemTrayIcon::DoubleClick == active)
        {
            if (this->isVisible())
            {
                if (this->isMinimized())
                {
                    this->showNormal();
                    this->raise();
                    this->activateWindow();
                }
                else
                {
                    this->hide();
                }

            }
            else
            {
                this->show();
                this->raise();
                this->activateWindow();
            }
        }
    });
    connect(ser,&TcpServer::userErro,[&](){
        QMessageBox::warning(this,tr("登录失败"),tr("登录失败，用户名或密码错误！"));
        this->ui->butLogin->setEnabled(true);
    });
    connect(ser,&TcpServer::notToServer,[&](){
        QMessageBox::warning(this,tr("网络错误"),tr("无法链接到服务器，请检查您的配置！"));
        this->ui->butLogin->setEnabled(true);
        trayIcon->setIcon(QIcon(":/ico/un"));
        trayIcon->setToolTip(tr("未连接到服务器！"));
    });
    connect(ser,&TcpServer::listenState,[&](bool lis){
        if (lis)
        {
            this->ui->stackedWidget->setCurrentIndex(1);
            trayIcon->setIcon(QIcon(":/ico/ed"));
            trayIcon->setToolTip(tr("已连接"));
        }
        else
        {
            QMessageBox::warning(this,tr("配置错误"),tr("本地端口监听失败，请检查您的配置！"));
            this->ui->butLogin->setEnabled(true);
        }
    });
    connect(this->ui->butHidde, &QPushButton::clicked, this, &MainWindow::hide);
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butLogin_clicked()
{
    this->ui->butLogin->setEnabled(false);
    ser->setInfo(this->ui->lineServer->text(),this->ui->serPort->value(),this->ui->loaPort->value());
    ser->socketConnect(this->ui->lineUser->text(),this->ui->linepword->text());
}
