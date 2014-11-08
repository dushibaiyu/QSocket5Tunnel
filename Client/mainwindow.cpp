#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ser = new TcpServer(QString("127.0.0.1"),8888);
    ser->socketConnect("admin","admin");
}

MainWindow::~MainWindow()
{
    delete ui;
}
