#ifndef CIPHER_H
#define CIPHER_H

#include <QString>
#include <iostream>
#include <chrono>
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>

class Cipher {
public:

    Cipher(const Cipher&) = delete;
    Cipher& operator=(const Cipher&) = delete;

    static Cipher& instance() {
        static Cipher controller;
        return controller;
    }


    std::pair<QString, QString> aesEncrypt(const QString& plainText);
    QString aesDecrypt(const QString& encryptedText, const QString& salt);

private:
    Cipher();

    CryptoPP::SecByteBlock key{32};
};


#endif // CIPHER_H
