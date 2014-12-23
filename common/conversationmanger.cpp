#include "conversationmanger.h"
#include "opensslaes.h"

QEvent::Type NewConEvent::EventType = (QEvent::Type)QEvent::registerEventType();

ConversationManger::ConversationManger(const QString &key, int maxCache, QObject *parent)
    : QObject(parent),max(maxCache)
{
    aes = new OpensslAES(key.toUtf8());
}

ConversationManger::~ConversationManger()
{
    delete aes;
}

void ConversationManger::newCache(Conversation * con)
{
    if (cacheCon.size() >=  max)
        con->deleteLater();
    else
        cacheCon.append(con);
}

void ConversationManger::customEvent(QEvent *event)
{
    if (event->type() == NewConEvent::EventType) {
        auto ev = static_cast<NewConEvent *>(event);
        Conversation * con;
        if (cacheCon.isEmpty()) {
            con = newCConversation();
        } else {
            con = cacheCon.dequeue();
        }
        if (!con->setDescriptor(ev->getSocketNofy()))
            cacheCon.append(con);
        ev->accept();
    } else {
        QObject::customEvent(event);
    }
}
