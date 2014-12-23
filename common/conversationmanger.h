#ifndef CONVERSATIONMANGER_H
#define CONVERSATIONMANGER_H

#include <QObject>
#include <QQueue>
#include <QEvent>
#include "conversation.h"

class ConversationManger : public QObject
{
    Q_OBJECT
public:
    explicit ConversationManger(const QString & key,int maxCache,QObject *parent = 0);
    ~ConversationManger();

protected slots:
    void newCache(Conversation *con);
protected:
    void customEvent(QEvent * event);
    virtual Conversation * newCConversation() = 0;
    OpensslAES * aes;
private:
    QQueue<Conversation *> cacheCon;//缓存的练剑池
    int max;
};

class NewConEvent : public QEvent
{
public:
    NewConEvent(): QEvent(EventType){}
    static const QEvent::Type EventType;
    qint16 getSocketNofy() const {return this->mofy;}
    void setSocketNofy(qint16 nofy) {this->nofy = nofy;}
private:
    qint16 mofy;
};

#endif // CONVERSATIONMANGER_H
