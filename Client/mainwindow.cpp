#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSystemTrayIcon>
#include "configclass.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    trayIcon = new QSystemTrayIcon(QIcon(":/ico/un"),this);
    initUI();
    ser = new TcpServer(this);
    trayIcon->show();
    this->ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (this->ui->lineServer->text().isEmpty() || this->ui->password->text().isEmpty())
    {
        QMessageBox::warning(this,tr("信息不全"),tr("请先完善必须信息！"));
        return;
    }
    ConfigClass & config = ConfigClass::getClass();
    config.localPort = this->ui->loaPort->value();
    config.serverPort = this->ui->serPort->value();
    config.serverUrl = this->ui->lineServer->text();
    config.key = this->ui->password->text();
    config.MaxCache = this->ui->maxcache->value();
    config.MaxThread = this->ui->maxthread->value();
    if (ser->listen(QHostAddress::Any,config.localPort))
        this->ui->stackedWidget->setCurrentIndex(1);
    else
        QMessageBox::warning(this,tr("错误"),tr("本地监听失败！是不已经有程序运行，还是其他程序占用端口！"));
}


void MainWindow::initUI()
{
    trayIcon->setToolTip(tr("未连接到服务器！"));
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
    connect(this->ui->butHidde, &QPushButton::clicked, this, &MainWindow::hide);
    ConfigClass & config = ConfigClass::getClass();
    this->ui->loaPort->setValue(config.localPort);
    this->ui->serPort->setValue(config.serverPort);
    this->ui->lineServer->setText(config.serverUrl);
    this->ui->password->setText(config.key);
    this->ui->maxcache->setValue(config.MaxCache);
    this->ui->maxthread->setValue(config.MaxThread);
}
