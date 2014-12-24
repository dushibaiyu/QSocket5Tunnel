#include "serverconver.h"
#include <QTcpSocket>
#include <QtEndian>
#include <QHostAddress>
#include "../common/datastruct.h"
#include "configclass.h"
#include "serverconver.h"

ServerConver::ServerConver(OpensslAES *aes, QObject *parent)
    : Conversation(aes,parent)
{

}

ServerConver::~ServerConver()
{

}

bool ServerConver::initConSocket(qint16 socket)
{
    socket2->setSocketDescriptor(socket);
    socket2->waitForReadyRead(10000);
    QByteArray data;
    if (socket2->bytesAvailable() >= 4){
        data = socket2->read(4);
    }
    int size = data.toInt(0,16);
    if (size > 0) {
        if (socket2->bytesAvailable() < size)
            socket2->waitForReadyRead(2000);
        data = socket2->read(size);
        if (decryptData(aes,data)) {
            QString sdata(data);
            QStringList dlist = sdata.split("\n");
            if (dlist.size() >= 2){
                size = dlist.at(1).toInt();
                if (size > 0) {
                    socket1->connectToHost(dlist.at(0),size);
                    if (socket1->waitForConnected(5000))
                        return true;
                }
            }
        }
    }
    socket2->disconnectFromHost();
    return false;
}
