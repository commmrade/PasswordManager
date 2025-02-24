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


    QString aesEncrypt(const QString& plainText);
    QString aesDecrypt(const QString& encryptedText);

private:
    Cipher();

    // Static key and IV initialized directly
    CryptoPP::SecByteBlock key{32};
    CryptoPP::byte iv[16];

};


#endif // CIPHER_H
