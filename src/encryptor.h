#pragma once
#include <string>

class Encryptor {
public:
    static bool encryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password);
    static bool decryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password);
};
