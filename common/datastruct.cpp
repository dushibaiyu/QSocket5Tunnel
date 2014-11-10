#include "datastruct.h"

QDataStream & operator<< (QDataStream & out, swapData & data)
{
    out << data.operater << data.userID << data.socketID << data.data;
    return out;
}

QDataStream & operator >> (QDataStream & in, swapData & data)
{
    in >> data.operater >> data.userID >> data.socketID >> data.data;
    return in;
}

BotanAES::BotanAES(const QByteArray & pass,const QByteArray & salt)
{
    encry.recreateKeys(pass,salt);
    decry.recreateKeys(pass,salt);
}

OpensslAES::OpensslAES(const QByteArray &pass, const QByteArray &salt):
    aes(pass,salt)
{}
