#include <QCoreApplication>
#include "tcpserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpServer ser;
    if (ser.listen(QHostAddress::Any,ConfigClass::getClass().port))
        return a.exec();
    else
        return 0;
}
