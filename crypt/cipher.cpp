#include "cipher.h"
#include <iostream>
#include <chrono>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <QDebug>
#include <QSettings>
#include "passwordgenerator.h"

Cipher::Cipher() {
    QSettings settings;
    auto rawPassword = settings.value("security/password").toString().toUtf8();
    key = CryptoPP::SecByteBlock{(const unsigned char*)rawPassword.constData(), static_cast<size_t>(rawPassword.size())};
}


std::pair<QString, QString> Cipher::aesEncrypt(const QString &plain)
{
    std::string cipher;
    std::string output;

    auto ivStr = PasswordGenerator::generatePswd(); // default is 16
    CryptoPP::byte iv[IV_SIZE];
    std::memcpy(iv, ivStr.toUtf8().constData(), ivStr.size());

    std::string plainStd = plain.toStdString();
    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e(key, key.size(), iv);
        CryptoPP::StringSource(plainStd, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink(cipher)));
    } catch (CryptoPP::Exception &exception) {
        qDebug() << "Encrypting" << exception.what();
    }
    CryptoPP::StringSource(cipher, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output)));
    return {QString::fromStdString(output), ivStr};
}

QString Cipher::aesDecrypt(const QString &encryptedText, const QString& salt)
{
    std::string cipher;
    std::string output;

    std::string encoded = encryptedText.toStdString();

    CryptoPP::byte iv[16];
    std::memcpy(iv, salt.toUtf8().constData(), salt.size());

    CryptoPP::StringSource(encoded, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(cipher)));
    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d(key, key.size(), iv);
        CryptoPP::StringSource(cipher, true, new CryptoPP::StreamTransformationFilter(d, new CryptoPP::StringSink(output)));
    } catch (CryptoPP::Exception &exception) {
        qDebug() << "Decrypting" << exception.what();
    }
    QString outputStr = QString::fromStdString(output);
    return outputStr;
}

