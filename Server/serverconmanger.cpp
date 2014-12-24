#include "serverconmanger.h"
#include "serverconver.h"
#include <QThread>

ServerConManger::ServerConManger(const QString & key,int maxCache,QObject *parent)
    : ConversationManger(key,maxCache,parent)
{
    th = new QThread;
    th->start();
    this->moveToThread(th);
}

ServerConManger::~ServerConManger()
{
    th->quit();
    th->wait();
    delete th;
}

Conversation * ServerConManger::newCConversation()
{
    auto con = new ServerConver(aes,this);
    connect(con,&ServerConver::connectionClosed,this,&ServerConManger::newCache);
    return con;
}
