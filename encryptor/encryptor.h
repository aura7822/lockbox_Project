#pragma once
#include <string>
#include namespace std;
class Encryptor{
     public:
       static bool encryptFile(const string& inputPath, const string& outputPath, const string& password);
       static bool decryptFile(const string& inputPath, const string& outputPath, const string& password);
};
