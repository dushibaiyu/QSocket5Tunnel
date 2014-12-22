/**
  Based on http://saju.net.in/code/misc/openssl_aes.c.txt example

  public domain

  Author Ruslan Salikhov <ruslansalihov /at/ gmail.com>
**/
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "opensslaes.h"

#include <QByteArray>
#include <QCryptographicHash>

OpensslAES::OpensslAES(const QByteArray &key, const QByteArray &salt, Type type, Mode mode)
    : m_isValid(false),
      m_type(type),
      m_mode(mode)
{
    m_blockSize = calcBlockSize();

    /* gen key and iv. init the cipher ctx object */
    m_isValid = init(key, salt);
}


OpensslAES::~OpensslAES()
{
    EVP_CIPHER_CTX_cleanup(&m_encoder);
    EVP_CIPHER_CTX_cleanup(&m_decoder);
}

/**
 * Create an 256 bit key and IV using the supplied key_data. salt can be added for taste.
 * Fills in the encryption and decryption ctx objects and returns 0 on success
 **/
bool OpensslAES::init(const QByteArray &keyData,
                         const QByteArray &saltData)
{
    int i, nrounds = 5;
    unsigned char key[32], iv[32];

    const EVP_CIPHER *evpCipher = getEvpCipher();

    if (evpCipher == 0)
        return false;

    /* 8 bytes (required EVP_BytesToKey) to salt the key_data during key generation. */
    unsigned char salt[8] = {0xAD, 0x7E, 0xF8, 0x14, 0x92, 0x31,0x2B, 0x3F};
    QByteArray saltDataHash = QCryptographicHash::hash(saltData, QCryptographicHash::Md5);
    memcpy(salt, saltDataHash.constData(), 8);

    /*
     * Gen key & IV for choosen algoritm. A SHA1 digest is used to hash the supplied key material.
     * nrounds is the number of times the we hash the material. More rounds are more secure but
     * slower.
     */
    i = EVP_BytesToKey(evpCipher, EVP_sha1(), salt,
                       (const unsigned char*) keyData.constData(), keyData.size(),
                       nrounds, key, iv);
    if (i*8 != m_blockSize) { // i*8 = blocksize in bits
        printf("Key size is %d bytes - should be %d bits\n", i, m_blockSize);
        return false;
    }

    EVP_CIPHER_CTX_init(&m_encoder);
    EVP_EncryptInit_ex(&m_encoder, evpCipher, 0, key, iv);
    EVP_CIPHER_CTX_init(&m_decoder);
    EVP_DecryptInit_ex(&m_decoder, evpCipher, 0, key, iv);

    return true;
}

const EVP_CIPHER *OpensslAES::getEvpCipher() const
{
    if (m_type == TypeAes128 && m_mode == ModeCbc) {
        return EVP_aes_128_cbc();
    } else if (m_type == TypeAes128 && m_mode == ModeCfb) {
        return EVP_aes_128_cfb128();
    } else if (m_type == TypeAes128 && m_mode == ModeEcb) {
        return EVP_aes_128_ecb();
    } else if (m_type == TypeAes128 && m_mode == ModeOfb) {
        return EVP_aes_128_ofb();
    } else if (m_type == TypeAes192 && m_mode == ModeCbc) {
        return EVP_aes_192_cbc();
    } else if (m_type == TypeAes192 && m_mode == ModeCfb) {
        return EVP_aes_192_cfb128();
    } else if (m_type == TypeAes192 && m_mode == ModeEcb) {
        return EVP_aes_192_ecb();
    } else if (m_type == TypeAes192 && m_mode == ModeOfb) {
        return EVP_aes_192_ofb();
    } else if (m_type == TypeAes256 && m_mode == ModeCbc) {
        return EVP_aes_256_cbc();
    } else if (m_type == TypeAes256 && m_mode == ModeCfb) {
        return EVP_aes_256_cfb128();
    } else if (m_type == TypeAes256 && m_mode == ModeEcb) {
        return EVP_aes_256_ecb();
    } else if (m_type == TypeAes256 && m_mode == ModeOfb) {
        return EVP_aes_256_ofb();
    }

    return 0;
}

int OpensslAES::calcBlockSize() const
{
    switch (m_type) {
        case TypeAes128:
            return 128;
        case TypeAes192:
            return 192;
        case TypeAes256:
            return 256;
        default:
            return 0;
    }
}


/**
 * Encrypt data
 **/
QByteArray OpensslAES::encrypt(const QByteArray &plainData)
{
    if (!m_isValid)
        return QByteArray();

    /* max ciphertext len for a n bytes of plaintext is n + AES_BLOCK_SIZE -1 bytes */
    int c_len = plainData.size() + m_blockSize - 1, f_len = 0;
    unsigned char *ciphertext = (unsigned char *)malloc(c_len);

    /* allows reusing of 'e' for multiple encryption cycles */
    if (!EVP_EncryptInit_ex(&m_encoder, 0, 0, 0, 0)) {
        printf("ERROR in EVP_EncryptInit_ex \n");
        return QByteArray();
    }

    /* update ciphertext, c_len is filled with the length of ciphertext generated,
        *len is the size of plaintext in bytes */
    if (!EVP_EncryptUpdate(&m_encoder, ciphertext, &c_len,
                           (const unsigned char *)plainData.constData(), plainData.size())) {
        printf("ERROR in EVP_EncryptUpdate \n");
        return QByteArray();
    }

    /* update ciphertext with the final remaining bytes */
    if (!EVP_EncryptFinal_ex(&m_encoder, ciphertext+c_len, &f_len)) {
        printf("ERROR in EVP_EncryptFinal_ex \n");
        return QByteArray();
    }

    return QByteArray((const char*)ciphertext, c_len + f_len);
}

/**
 * Decrypt cipher data
 **/
QByteArray OpensslAES::decrypt(const QByteArray &cipherData)
{
    if (!m_isValid)
        return QByteArray();

    /* plaintext will always be equal to or lesser than length of ciphertext*/
    int p_len = cipherData.size(), f_len = 0;
    unsigned char *plaintext = (unsigned char *)malloc(p_len);

    if (!EVP_DecryptInit_ex(&m_decoder, 0, 0, 0, 0)) {
        printf("ERROR in EVP_DecryptInit_ex\n");
        return QByteArray();
    }

    if (!EVP_DecryptUpdate(&m_decoder, plaintext, &p_len,
                           (const unsigned char*)cipherData.constData(), cipherData.size())) {
        printf("ERROR in EVP_DecryptUpdate\n");
        return QByteArray();
    }

    if (!EVP_DecryptFinal_ex(&m_decoder, plaintext+p_len, &f_len)) {
        printf("ERROR in EVP_DecryptFinal_ex\n");
        return QByteArray();
    }

    return QByteArray((const char*)plaintext, p_len + f_len);
}

bool OpensslAES::isValid() const
{
    return m_isValid;
}

OpensslAES::Type OpensslAES::type() const
{
    return m_type;
}

OpensslAES::Mode OpensslAES::mode() const
{
    return m_mode;
}

int OpensslAES::blockSize() const
{
    return m_blockSize;
}



