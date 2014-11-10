/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "sshcryptofacility_p.h"

#include "sshbotanconversions_p.h"
#include <botan/botan.h>

#include <QDebug>
#include <QList>

#include <string>

using namespace Botan;


SshAbstractCryptoFacility::SshAbstractCryptoFacility()
    : m_cipherBlockSize(0), m_macLength(0)
{
}

SshAbstractCryptoFacility::~SshAbstractCryptoFacility() {}

void SshAbstractCryptoFacility::clearKeys()
{
    m_cipherBlockSize = 0;
    m_macLength = 0;
    m_sessionId.clear();
    m_pipe.reset(0);
    m_hMac.reset(0);
}

void SshAbstractCryptoFacility::recreateKeys(const QByteArray &pass, const QByteArray &salt)
{
    checkInvariant();

    if (m_sessionId.isEmpty())
        m_sessionId = pass;
    Algorithm_Factory &af = global_state().algorithm_factory();
    Botan::HashFunction *hash = af.make_hash_function("SHA-1");
    BlockCipher * const cipher = af.prototype_block_cipher("AES-128")->clone();

    m_cipherBlockSize = cipher->block_size();
    const QByteArray ivData = generateHash(hash,pass,salt, ivChar(), m_cipherBlockSize);
    const InitializationVector iv(convertByteArray(ivData), m_cipherBlockSize);

    const quint32 keySize = cipher->key_spec().maximum_keylength();
    const QByteArray cryptKeyData = generateHash(hash,pass,salt, keyChar(), keySize);
    SymmetricKey cryptKey(convertByteArray(cryptKeyData), keySize);

    Keyed_Filter * const cipherMode = makeCipherMode(cipher, new Null_Padding, iv, cryptKey);
    m_pipe.reset(new Pipe(cipherMode));
}

bool SshAbstractCryptoFacility::convert(QByteArray &data,
    quint32 dataSize) const
{
    checkInvariant();

    // Session id empty => No key exchange has happened yet.
    if (dataSize == 0 || m_sessionId.isEmpty())
        return false;

    if (dataSize % cipherBlockSize() != 0) {
        return false;
    }
    m_pipe->process_msg(reinterpret_cast<const byte *>(data.constData()),
        dataSize);
    quint32 bytesRead = m_pipe->read(reinterpret_cast<byte *>(data.data()),
        dataSize, m_pipe->message_count() - 1); // Can't use Pipe::LAST_MESSAGE because of a VC bug.
    if (bytesRead != dataSize) {
       return false;
    }
    return true;
}

void SshAbstractCryptoFacility::checkInvariant() const
{
    Q_ASSERT(m_sessionId.isEmpty() == !m_pipe);
}

QByteArray SshAbstractCryptoFacility::generateHash(Botan::HashFunction *hash,const QByteArray &pass, const QByteArray &salt,
    char c, quint32 length)
{
    const QByteArray &k = pass;
    const QByteArray &h = salt;
    QByteArray data(k);
    data.append(h).append(c).append(m_sessionId);
    SecureVector<byte> key
        = hash->process(convertByteArray(data), data.size());
    while (key.size() < length) {
        SecureVector<byte> tmpKey;
        tmpKey += SecureVector<byte>(convertByteArray(k), k.size());
        tmpKey += SecureVector<byte>(convertByteArray(h), h.size());
        tmpKey += key;
        key += hash->process(tmpKey);
    }
    return QByteArray(reinterpret_cast<const char *>(key.begin()), length);
}

Keyed_Filter *SshEncryptionFacility::makeCipherMode(BlockCipher *cipher,
    BlockCipherModePaddingMethod *paddingMethod, const InitializationVector &iv,
    const SymmetricKey &key)
{
    return new CBC_Encryption(cipher, paddingMethod, key, iv);
}

void SshEncryptionFacility::encrypt(QByteArray &data) const
{
    convert(data,  data.size());
}


SshEncryptionFacility::~SshEncryptionFacility() {}


Keyed_Filter *SshDecryptionFacility::makeCipherMode(BlockCipher *cipher,
    BlockCipherModePaddingMethod *paddingMethod, const InitializationVector &iv,
    const SymmetricKey &key)
{
    return new CBC_Decryption(cipher, paddingMethod, key, iv);
}

void SshDecryptionFacility::decrypt(QByteArray &data) const
{
    convert(data,data.size());
}

