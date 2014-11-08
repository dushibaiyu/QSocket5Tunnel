#ifndef TcpServer_H
#define TcpServer_H

#include <QTcpServer>
#include <QHash>
#include "localsocket.h"
#include "../common/datastruct.h"


class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QString serHost,qint16 serPort,qint16 localBind = 6666,QObject *parent = 0);
    ~TcpServer();
signals:
    void notToServer();
    void userErro();
    void listenState(bool);

public slots:
    void socketConnect(const QString & user,const QString & pass);

    void serSocketRead();
    void serSocketDisCon();

    void localSockedDisCon();//断开连接的用户信息
    void LocalSocketRead();

protected:
    void incomingConnection(qintptr socketDescriptor);
    void initLocalProxy(QString & thisHost,qint16 & thisPort,LocalSocket * sock);

    void handleSwapData(swapData & data);
    void handleUserLog(swapData & data);
    void handleDisCon(swapData & data);
private:
    QHash<int,LocalSocket *> * tcpClient;//管理连接的map
    qint32 userID = -1;//用户ID
    QByteArray tocken;
    QTcpSocket * serverSocket;
    bool isSerCon;
    QString serHost;
    qint16 serPort;
    qint16 localBind;
    qulonglong lastsize;
};

#endif // TcpServer_H
