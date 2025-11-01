#pragma once
#include <string>

class ImgUtils {
public:
    static bool hideMessage(const std::string &inputImage,
                            const std::string &message,
                            const std::string &outputImage);
    static std::string extractMessage(const std::string &inputImage);
};
