#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "tcpserver.h"
#include <QCoreApplication>

TcpServer::TcpServer(QObject *parent) :
    QTcpServer(parent),cutsize(0)
{}

TcpServer::~TcpServer()
{
    clear();
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    if (manger.isEmpty()) {
        ConfigClass & conf = ConfigClass::getClass();
        for (int i = 0; i < conf.MaxThread; ++i)
        {
            manger.append(new ClientConManger(conf.key,conf.MaxCache));
        }
    }
    goFawrd();
    auto tmp = new NewConEvent;
    tmp->setSocketNofy(socketDescriptor);
    QCoreApplication::postEvent(manger[cutsize],tmp);

}

void TcpServer::clear()
{
    for (int i = 0; i < manger.size(); ++i) {
        auto tmp = manger.at(i);
        delete tmp;
    }
}
