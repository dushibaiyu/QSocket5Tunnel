#ifndef ClientSocket_H
#define ClientSocket_H

#include "remotesocket.h"
#include "../common/datastruct.h"
#include <QHash>

class ClientSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ClientSocket(qintptr socketDescriptor,QObject *parent = 0);
    ~ClientSocket();

signals:
    void sockDisConnect(const int ,const QString &,const quint16, QThread *);//NOTE:断开连接的用户信息，此信号必须发出！线程管理类根据信号计数的
public slots:
    void sentData(const QByteArray & ,const int);//发送信号的槽
    void disConTcp(int i);

    void deleteThis();
protected slots:
    void remoteData();
    void remoteDisCon();
    void clientData();
protected:
    void handleSwapData();
    void handleUserLog();
    void handleNewCon();
    void handleDisCon();

    inline bool sentClientData();

    bool decryptClientData(swapData & data);
private:
    qintptr socketID;
    QHash<int,RemoteSocket *> socketList;
    qint32 userID;
    QString token;
    qulonglong lastsize;
    OpensslAES * aes;
private://临时变量，放到类里，优化每次分配
    swapData data;
    QByteArray bytearry,basize;
    QPair<QString , qint16> newHost;
};

#endif // ClientSocket_H
