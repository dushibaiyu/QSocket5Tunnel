#ifndef CLIENTCONMANGER_H
#define CLIENTCONMANGER_H

#include "../common/conversationmanger.h"

class ClientConManger : public ConversationManger
{
    Q_OBJECT
public:
    explicit ClientConManger(const QString & key,int maxCache,QObject *parent = 0);
    ~ClientConManger();
protected:
    Conversation * newCConversation();
private:
    QThread * th;
};

#endif // CLIENTCONMANGER_H
