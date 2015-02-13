#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QCoreApplication>
#include "threadserver.h"
#include "threadhandle.h"
#include "userconfig.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SysConfig syscon;

    ThreadServer ser(syscon.getMaxThread());
    ser.listen(syscon.getBindPort());
    return a.exec();
}
