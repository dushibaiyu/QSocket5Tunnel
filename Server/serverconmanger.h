#ifndef SERVERCONMANGER_H
#define SERVERCONMANGER_H

#include "../common/conversationmanger.h"

class ServerConManger : public ConversationManger
{
    Q_OBJECT
public:
    explicit ServerConManger(const QString & key,int maxCache,QObject *parent = 0);
    ~ServerConManger();
protected:
    Conversation * newCConversation();
private:
    QThread * th;
};

#endif // CLIENTCONMANGER_H
