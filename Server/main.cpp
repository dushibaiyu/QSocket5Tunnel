#include <QCoreApplication>
#include <tcpserver.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    TcpServer ser;
    ser.listen(QHostAddress::Any,8888);
    return a.exec();
}
