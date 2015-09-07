#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
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
//BUG:序列化和反序列化有问题！！！
inline QByteArray serializeData(const QAesWrap & aes,OperaterType type,int id,const QByteArray & data)
{
    uint size = data.size() + 5;
    char str[4] = {0};
    QByteArray rdata(size + 4,0);
    char * rd = rdata.data();
    qToBigEndian(size,(uchar *)str);
    memcpy(rd,str,4);
    rd += 4;
    QByteArray enData(size, 0);
    char * ed = enData.data();
    ed[0] = (char)type;
    ++ ed;
    memset(str,0,4);
    uint uid = (uint)id;
    qToBigEndian(uid,(uchar *)str);
    memcpy(ed,str,4);
    ed += 4;
    memcpy(ed,data.data(),data.size());
    QByteArray out;
    aes.encrypt(enData,out,QAesWrap::AES_CTR);
    memcpy(rd,out.data(),out.size() );
    return rdata;
}

inline QByteArray deserializeData(const QAesWrap & aes,OperaterType & type,int & id,const QByteArray & data)
{
    QByteArray rdata;
    aes.decrypt(data,rdata,QAesWrap::AES_CTR);
    char str[4] = {0};
    type = OperaterType((int)rdata.at(0));
    char * rd = rdata.data();
    ++ rd;
    memcpy(str,rd,4);
    uint uid = qFromBigEndian<int>((uchar*)str);
    id  = (int)uid;
    return rdata.mid(5);
}

#endif // DATASTRUCT_H
