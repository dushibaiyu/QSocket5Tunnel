#ifndef SERVERCONVER_H
#define SERVERCONVER_H

#include "../common/conversation.h"

class ServerConver : public Conversation
{
    Q_OBJECT
public:
    explicit ServerConver(OpensslAES * aes,QObject *parent = 0);
    ~ServerConver();

signals:

public slots:

protected:
    bool initConSocket(qint16 socket);
};

#endif // CLIENTCONVER_H
