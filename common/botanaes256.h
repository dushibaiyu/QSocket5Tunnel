#ifndef BotanAES256_H
#define BotanAES256_H
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <memory>
#include <botan.h>

using namespace std;
using namespace Botan;
class BotanAES256 : public QObject
{
    Q_OBJECT
public:
    explicit BotanAES256(QString password = "dushibaiyu",QString salt = "http://www.dushibaiyu.com",QObject *parent = 0);
    /*!
    * Creates a hash
    * @param Data The string to hash
    */
    QString Hash(const QString & Data);
    /*!
    * Creates a hash in hex format
    * @param Data The string to hash
    */
    QString HexHash(const QString &Data);
    /*!
    * Returns a Base64 encrypted QString
    * @param Data The string to encypt
    */
    QByteArray Encrypt(const QByteArray &Data);
    /*!
    * Returns a decrypted string from a Base64 encypted string
    * @param Data The string to encypt
    */
    QByteArray Decrypt(const QByteArray &Data);
    /*!
    * Encrypts a file and returns a bool indicating success
    * @param Source The source file
    * @param Destination The destination file
    */
    bool EncryptFile(QString Source, QString Destination);
    /*!
    * Decrypts a file and returns a bool indicating success
    * @param Source The source file
    * @param Destination The destination file
    */
    bool DecryptFile(QString Source, QString Destination);
signals:
public slots:
private:
    /*!
    * The botan libary initilizer
    */
    Botan::LibraryInitializer mInit;
    /*!
    * The Salt
    */
    SecureVector<byte> mSalt;
    /*!
    * The password
    */
    std::string mPassword;
};
#endif // BotanAES256_H 
