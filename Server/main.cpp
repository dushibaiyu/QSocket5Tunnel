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

    Server ser;
    qDebug() << ser.Listen(QString(),2048);
    return a.exec();
}
