#ifndef TcpServer_H
#define TcpServer_H

#include <QTcpServer>
#include <QHash>
#include "configclass.h"


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();
signals:
    void notToServer();
    void userErro();
    void listenState(bool);

public slots:
    void socketConnect();

    void serSocketRead();
    void serSocketDisCon();

    void localSockedDisCon();//断开连接的用户信息
    void LocalSocketRead();

protected:
    void incomingConnection(qintptr socketDescriptor);
    void initLocalProxy(QString & thisHost,qint16 & thisPort,LocalSocket * sock);//socket5代理协议

    void handleSwapData();
    void handleUserLog();
    void handleDisCon();

    inline bool sentServerData();
private:
    QHash<int,LocalSocket *> * tcpClient;//管理连接的map
    qint32 userID;// = -1;//用户ID
    QString tocken;
    QTcpSocket * serverSocket;
    bool isSerCon;
    qulonglong lastsize;
    OpensslAES * aes;

private://临时变量，放到类里，优化每次分配
    swapData data;
    QByteArray bytearry,basize;
    QPair<QString , qint16> newHost;
};

#endif // TcpServer_H
