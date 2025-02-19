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

Cipher::Cipher() {
    CryptoPP::AutoSeededRandomPool rnd;
    rnd.GenerateBlock(key, key.size());
    rnd.GenerateBlock(iv, sizeof(iv));
}

QString Cipher::aesEncrypt(const QString &plain)
{
    std::string cipher;
    std::string output;

    std::string plainStd = plain.toStdString();
    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e(key, key.size(), iv);
        CryptoPP::StringSource(plainStd, true, new CryptoPP::StreamTransformationFilter(e, new CryptoPP::StringSink(cipher)));
    } catch (CryptoPP::Exception &exception) {
        qDebug() << exception.what();
    }

    CryptoPP::StringSource(cipher, true, new CryptoPP::HexEncoder(new CryptoPP::StringSink(output)));
    return QString::fromStdString(output);
}

QString Cipher::aesDecrypt(const QString &encryptedText)
{
    std::string cipher;
    std::string output;

    std::string encoded = encryptedText.toStdString();

    CryptoPP::StringSource(encoded, true, new CryptoPP::HexDecoder(new CryptoPP::StringSink(cipher)));

    try {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d(key, key.size(), iv);
        CryptoPP::StringSource(cipher, true, new CryptoPP::StreamTransformationFilter(d, new CryptoPP::StringSink(output)));
    } catch (CryptoPP::Exception &exception) {
        qDebug() << exception.what();
    }
    return QString::fromStdString(output);
}

