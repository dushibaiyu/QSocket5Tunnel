#ifndef CLIENTCONVER_H
#define CLIENTCONVER_H

#include "../common/conversation.h"

class ClientConver : public Conversation
{
    Q_OBJECT
public:
    explicit ClientConver(OpensslAES * aes,QObject *parent = 0);
    ~ClientConver();

signals:

public slots:

protected:
    bool initConSocket(qint16 socket);

private:
    bool initLocalProxy(QString & thisHost,qint16 & thisPort,QTcpSocket * sock);
};

#endif // CLIENTCONVER_H
