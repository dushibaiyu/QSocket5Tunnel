#ifndef AES_H
#define AES_H

#include <QByteArray>

typedef unsigned char byte;
class QFile;

class AES
{
public:
    enum KeySize // key size, in bits, for construtor
    {
        Bits128 = 16,
        Bits192 = 24,
        Bits256 = 32
    };

    AES(const QByteArray & password = "dushibaiyu", const QByteArray & salt = "www.dushibaiyu.com", AES::KeySize keySize = Bits256);
    ~AES();


    void fileEncrypt(QFile *fin, QFile *fout);
    void fileDecrypt(QFile *fin, QFile *fout);
    void dataEncrypt(QByteArray & data);
    void dataDecrypt(QByteArray & data);
protected:
    void Cipher(unsigned char* input, unsigned char* output);
    void InvCipher(unsigned char * input, unsigned char * output);
private:
    void RunKeyExpansion(KeySize keySize, unsigned char* keyBytes);
    void SetNbNkNr(KeySize keyS);
    void KeyExpansion();
    void AddRoundKey(int round);
    void SubWord(unsigned char * word,unsigned char* result);
    void RotWord(unsigned char * word,unsigned char* result);
    void SubBytes();
    void InvSubBytes();
    void ShiftRows();
    void InvShiftRows();
    void MixColumns();
    void InvMixColumns();
    unsigned char gfmultby01(unsigned char b);
    unsigned char gfmultby02(unsigned char b);
    unsigned char gfmultby03(unsigned char b);
    unsigned char gfmultby09(unsigned char b);
    unsigned char gfmultby0b(unsigned char b);
    unsigned char gfmultby0d(unsigned char b);
    unsigned char gfmultby0e(unsigned char b);

private:
    static const unsigned char Sbox[16*16];
    static const unsigned char iSbox[16*16];
    static const unsigned char Rcon[11*4];

    int Nb;         // block size in 32-bit words.  Always 4 for AES.  (128 bits).
    int Nk;         // key size in 32-bit words.  4, 6, 8.  (128, 192, 256 bits).
    int Nr;         // number of rounds. 10, 12, 14.

    unsigned char key[32];     // the seed key. size will be 4 * keySize from ctor.
    unsigned char w[16*15];      // key schedule array. (Nb*(Nr+1))*4
    unsigned char State[4][4];  // State matrix

};

#endif // AES_H
