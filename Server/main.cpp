#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QCoreApplication>
#include "server.h"
#include "userconfig.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    SysConfig syscon;

    int i = -64;
    uint fui = i;
    uint ui =(uint)i;
    uint tui;
    memcpy(&tui,&i,4);
    qDebug() << " i = " << i;
    qDebug() << " ui = " << ui;
    qDebug() << " fui = " << fui;
    qDebug() << " tui = " << tui;
    memset(&tui,4,4);
    qDebug() << " tui = " << tui;
    Server ser;
    qDebug() << ser.Listen(QString(),2048);
    return a.exec();
}
