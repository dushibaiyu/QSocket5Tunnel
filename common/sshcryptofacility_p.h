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

#ifndef SSHCRYPTOFACILITY_P_H
#define SSHCRYPTOFACILITY_P_H

#include <botan/botan.h>

#include <QByteArray>
#include <QScopedPointer>

class SshAbstractCryptoFacility
{
public:
    virtual ~SshAbstractCryptoFacility();

    void clearKeys();
    void recreateKeys(const QByteArray & pass, const QByteArray & salt);
    quint32 cipherBlockSize() const { return m_cipherBlockSize; }
protected:
    SshAbstractCryptoFacility();
    bool convert(QByteArray &data, quint32 dataSize) const;
    QByteArray sessionId() const { return m_sessionId; }

private:
    SshAbstractCryptoFacility(const SshAbstractCryptoFacility &);
    SshAbstractCryptoFacility &operator=(const SshAbstractCryptoFacility &);


    virtual Botan::Keyed_Filter *makeCipherMode(Botan::BlockCipher *cipher,
        Botan::BlockCipherModePaddingMethod *paddingMethod,
        const Botan::InitializationVector &iv,
        const Botan::SymmetricKey &key) = 0;
    virtual char ivChar() const = 0;
    virtual char keyChar() const = 0;

    QByteArray generateHash(Botan::HashFunction *hash, const QByteArray &pass, const QByteArray &salt, char c, quint32 length);
    void checkInvariant() const;

    QByteArray m_sessionId;
    QScopedPointer<Botan::Pipe> m_pipe;
    QScopedPointer<Botan::HMAC> m_hMac;
    quint32 m_cipherBlockSize;
    quint32 m_macLength;
};

class SshEncryptionFacility : public SshAbstractCryptoFacility
{
public:
    void encrypt(QByteArray &data) const;
    ~SshEncryptionFacility();

private:
    virtual Botan::Keyed_Filter *makeCipherMode(Botan::BlockCipher *cipher,
        Botan::BlockCipherModePaddingMethod *paddingMethod,
        const Botan::InitializationVector &iv, const Botan::SymmetricKey &key);
    virtual char ivChar() const { return 'A'; }
    virtual char keyChar() const { return 'C'; }

};

class SshDecryptionFacility : public SshAbstractCryptoFacility
{
public:
    void decrypt(QByteArray &data) const;

private:
    virtual Botan::Keyed_Filter *makeCipherMode(Botan::BlockCipher *cipher,
        Botan::BlockCipherModePaddingMethod *paddingMethod,
        const Botan::InitializationVector &iv, const Botan::SymmetricKey &key);
    virtual char ivChar() const { return 'B'; }
    virtual char keyChar() const { return 'D'; }

};

#endif // SSHCRYPTOFACILITY_P_H
