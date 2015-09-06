
#include <localsocket.h>

void LocalSocket::readData(const QByteArray & data)
{

    switch (state_) {
    case NeedCheck:
    {
        if(data.at(0)!= 5) {//只允许Socket5
            close();
            deleteLater();
            //            return;
        } else {
            char rep[2] = {0X05,0X00};
            local->write(rep,2);
            state_ = GetLink;
        }
    }
        break;
    case GetLink:
    {
        QByteArray * bufdata = const_cast<QByteArray *>(&data);
        QBuffer buf(bufdata);
        buf.open(QBuffer::ReadOnly);
        if (buf.bytesAvailable() < 6) {
            close();
            deleteLater();
            return;
        }
        repData.append((char)0x05); // SOCKS version
        repData.append((char)0x00);
        QByteArray ba = buf.read(4);
        if(ba.at(0)!= 5 || ba.at(1)!=1 || ba.at(2)!=0) {// only handle "open stream" requests
            close();
            deleteLater();
            return;
        }
        repData.append((char)0x00);
        repData.append(ba.at(3));
        QString host;
        switch(ba.at(3)) {
        // IPv4
        case 1: {
            if (buf.bytesAvailable() < 4) {
                close();
                deleteLater();
                return;
            }
            ba = buf.read(4);
            QIPAddressUtils::toString(host, qFromBigEndian<quint32>((uchar*)ba.constData()));
            repData.append(ba);
        }
            break;
            // domain name
        case 3: {
            ba = buf.read(1);
            int len = ba.at(0);
            if (buf.bytesAvailable() < len) {
                close();
                deleteLater();
                return;
            }
            repData.append(ba);
            ba = buf.read(len);
            host = ba;
            repData.append(ba);
        }
            break;
        case 4:{
            if (buf.bytesAvailable() < 16) {
                close();
                deleteLater();
                return;
            }
            ba = buf.read(16);
            QIPAddressUtils::IPv6Address  ip6;
            memcpy(ip6,ba.constData(),16);
            QIPAddressUtils::toString(host,ip6);
            repData.append(ba);
        }
            break;
        default: {
            close();
            deleteLater();
            return;
        }
        }

        if (buf.bytesAvailable() < 2) {
            close();
            deleteLater();
            return;
        }

        qDebug() << "new client";
        ba = buf.read(2);
        qint16 port = qFromBigEndian<qint16>((uchar*)ba.constData());
        host.append("@");
        host.append(host.number(port));
        repData.append(ba);
        state_ = Done;
        server->newConnet(host,id,this);
    }
        break;
    default:
    {
        server->write(serializeData(server->getAes(),SwapData,id,data));
    }
        break;
    }
}
