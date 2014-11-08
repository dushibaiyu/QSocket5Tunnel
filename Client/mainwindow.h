#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tcpserver.h"

namespace Ui {
class MainWindow;
}
//TODO：添加托盘图标，隐藏窗口，保存配置
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void on_butLogin_clicked();

private:
    Ui::MainWindow *ui;
    TcpServer * ser;
};

#endif // MAINWINDOW_H
