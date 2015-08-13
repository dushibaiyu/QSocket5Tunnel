#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include "opensslaes.h"
#include <qaeswrap.h>
#include <QtEndian>

#define DATASTREAM_VISION QDataStream::Qt_5_2
/****
 * operater : 值说明
 *          SwapData ：数据交换，就是socket5的数据转发
 *          NewLink ：新建链接，新建需要的远程链接，发想服务器由此状态，服务器返回无此状态
 *          DisLink ：断开链接。
 *          NeedKey ：请求加密key。
 * ****/

enum OperaterType {
    NeedKey = 0x01,
    NewLink = 0x02,
    DisLink = 0x03,
    SwapData = 0x04
};

/****
 * 数据格式：
 * 0000   [ 0  0000         0··········0 ]
 * 数据长度 操作 本地socketID  数据
 * 说明：每个0代表一个字节 。 [] 中的数据为加密数据
 * 前四个字节是数据长度，
 * 第5个字节是操作方式
 * 第6,7,8,9这4个字节是本地socket的id
 * 剩下的字节为数据。
 * 长度和socketID 是数字类型，以大端的方式转换的
*/

inline QByteArray serializeData(QAesWrap * aes,OperaterType type,int id,const QByteArray data)
{
    int size = data.size() + 5;
    char str[4] = {0};
    QByteArray rdata(size + 4,0);
    char * rd = rdata.data();
    rd[4] = (char)type;
    qToBigEndian(size,(uchar *)str);
    memcpy(rd,str,4);
    rd += 5;
    memset(str,0,4);
    qToBigEndian(id,(uchar *)str);
    memcpy(rd,str,4);
    rd += 4;
    QByteArray out;
    aes->encrypt(data,out,QAesWrap::AES_CTR);
    memcpy(rd,out.data(),size - 5 );
    return rdata;
}

deserializeData()
{}

#endif // DATASTRUCT_H
