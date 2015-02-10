#ifndef ClientSocket_H
#define ClientSocket_H

#include "remotesocket.h"
#include "../common/datastruct.h"
#include <QMap>

class ThreadServer;

class ClientSocket : public QAsioTcpSocket
{
    Q_OBJECT
public:
    ~ClientSocket();

signals:
    void sentDiscon(QThread * th,int id);

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
    inline void sentRemoteDisCon(int socketId);

protected:
    explicit ClientSocket(asio::ip::tcp::socket * socket, QObject *parent = 0);
    friend class ThreadServer;
    Q_DISABLE_COPY(ClientSocket)
private:
    int socketID;
    QMap<int,RemoteSocket *> socketList;
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
