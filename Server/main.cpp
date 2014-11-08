#include <QCoreApplication>
#include "tcpserver.h"
#include "threadhandle.h"
#include "userconfig.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SysConfig syscon;
    if (syscon.getThreadType())
        ThreadHandle::getClass().initThreadType(ThreadHandle::HANDLESIZE,syscon.getMaxThread());
    else
       ThreadHandle::getClass().initThreadType(ThreadHandle::THREADSIZE,syscon.getMaxThread());

    TcpServer ser;
    ser.listen(QHostAddress::Any,syscon.getBindPort());
    return a.exec();
}
