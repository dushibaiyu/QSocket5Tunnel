#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QObject>

class OpensslAES;
class QTcpSocket;

class Conversation : public QObject
{
    Q_OBJECT
public:
    explicit Conversation(OpensslAES * aes,QObject *parent = 0);
    bool setDescriptor(qint16 socket);
signals:
    void connectionClosed(Conversation * conv);
protected:
    void connectSockets();
    virtual bool initConSocket(qint16 socket) = 0;
protected slots:
    void socket1Read();
    void socket2Read();
    void socket1Closed();
    void socket2Closed();
protected:
    QTcpSocket * socket1;
    QTcpSocket * socket2;//这个是链接加密端数据的，所以客户端和服务端用的地方不一样
private:
    OpensslAES * aes;
};

#endif // CONVERSATION_H
