#include "encryptor.h"
#include <sodium.h>
#include <fstream>
#include <vector>
#include <iostream>

bool Encryptor::encryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    if (sodium_init() < 0) {
        std::cerr << "libsodium init failed\n";
        return false;
    }

    // Read file
    std::ifstream in(inputPath, std::ios::binary);
    if (!in) return false;
    std::vector<unsigned char> plain((std::istreambuf_iterator<char>(in)), {});
    in.close();

    unsigned char salt[crypto_pwhash_SALTBYTES];
    randombytes_buf(salt, sizeof(salt));

    unsigned char key[crypto_aead_xchacha20poly1305_ietf_KEYBYTES];
    if (crypto_pwhash(key, sizeof key, password.c_str(), password.size(),
                      salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        std::cerr << "Key derivation failed\n";
        return false;
    }

    unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES];
    randombytes_buf(nonce, sizeof nonce);

    std::vector<unsigned char> cipher(plain.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES);
    unsigned long long cipherLen;

    crypto_aead_xchacha20poly1305_ietf_encrypt(cipher.data(), &cipherLen,
        plain.data(), plain.size(),
        nullptr, 0, nullptr, nonce, key);

    std::ofstream out(outputPath, std::ios::binary);
    if (!out) return false;

    out.write((char*)salt, sizeof(salt));
    out.write((char*)nonce, sizeof(nonce));
    out.write((char*)cipher.data(), cipherLen);
    out.close();

    return true;
}

bool Encryptor::decryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    if (sodium_init() < 0) {
        std::cerr << "libsodium init failed\n";
        return false;
    }

    std::ifstream in(inputPath, std::ios::binary);
    if (!in) return false;

    unsigned char salt[crypto_pwhash_SALTBYTES];
    unsigned char nonce[crypto_aead_xchacha20poly1305_ietf_NPUBBYTES];
    in.read((char*)salt, sizeof(salt));
    in.read((char*)nonce, sizeof(nonce));

    std::vector<unsigned char> cipher((std::istreambuf_iterator<char>(in)), {});
    in.close();

    unsigned char key[crypto_aead_xchacha20poly1305_ietf_KEYBYTES];
    if (crypto_pwhash(key, sizeof key, password.c_str(), password.size(),
                      salt,
                      crypto_pwhash_OPSLIMIT_INTERACTIVE,
                      crypto_pwhash_MEMLIMIT_INTERACTIVE,
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        std::cerr << "Key derivation failed\n";
        return false;
    }

    std::vector<unsigned char> plain(cipher.size());
    unsigned long long plainLen;

    if (crypto_aead_xchacha20poly1305_ietf_decrypt(plain.data(), &plainLen,
            nullptr,
            cipher.data(), cipher.size(),
            nullptr, 0,
            nonce, key) != 0) {
        std::cerr << "Decryption failed: wrong password or corrupted file\n";
        return false;
    }

    std::ofstream out(outputPath, std::ios::binary);
    out.write((char*)plain.data(), plainLen);
    out.close();

    return true;
}
