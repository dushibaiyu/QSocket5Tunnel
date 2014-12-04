#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tcpserver.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QSystemTrayIcon;
//TODO：保存配置
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void on_butLogin_clicked();

protected:
    void connectSlots();
    void initUI();
private:
    Ui::MainWindow *ui;
    TcpServer * ser;
    QSystemTrayIcon * trayIcon;
};

#endif // MAINWINDOW_H
