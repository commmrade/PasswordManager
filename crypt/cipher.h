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
    Cipher();

    static QString aesEncrypt(const QString& plainText);
    static QString aesDecrypt(const QString& encryptedText);

private:
    // Static key and IV initialized directly
    static inline CryptoPP::SecByteBlock key{CryptoPP::AES::DEFAULT_KEYLENGTH};
    static inline CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];

};


#endif // CIPHER_H
