#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include "opensslaes.h"

inline bool decryptData( OpensslAES * aes,QByteArray & data)
{
#ifndef USE_ENCRYPT
    return true;
#endif
    if (data.isEmpty()) return false;
    data = aes->decrypt(data);
    if (data.isEmpty()) return false;
    return true;
}

inline QByteArray encryptData( OpensslAES * aes,const QByteArray &data)
{
    if (data.isEmpty()) return data;
#ifndef USE_ENCRYPT
    return data;
#endif
    return aes->encrypt(data);
}

#endif // DATASTRUCT_H
