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
    void setRemoteSocket(QTcpSocket*);
    void setLocalSocket(QTcpSocket*);
    void connectSockets();
    virtual bool initConSocket(qint16 socket) = 0;
protected slots:
    void remoteRead();
    void localRead();
    void remoteClosed();
    void localClosed();
private:
    QTcpSocket * socket1;
    QTcpSocket * socket2;
    OpensslAES * aes;
};

#endif // CONVERSATION_H
