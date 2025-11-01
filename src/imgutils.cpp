#include "imgutils.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

bool ImgUtils::hideMessage(const std::string &inputImage,
                           const std::string &message,
                           const std::string &outputImage) {
    cv::Mat image = cv::imread(inputImage);
    if (image.empty()) return false;

    int msgIndex = 0;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            if (msgIndex >= (int)message.size()) break;
            cv::Vec3b &pixel = image.at<cv::Vec3b>(i, j);
            pixel[0] = (pixel[0] & ~1) | ((message[msgIndex] >> 0) & 1);
            pixel[1] = (pixel[1] & ~1) | ((message[msgIndex] >> 1) & 1);
            pixel[2] = (pixel[2] & ~1) | ((message[msgIndex] >> 2) & 1);
            msgIndex++;
        }
    }

    return cv::imwrite(outputImage, image);
}

std::string ImgUtils::extractMessage(const std::string &inputImage) {
    cv::Mat image = cv::imread(inputImage);
    if (image.empty()) return "";

    std::string message;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
            char c = ((pixel[0] & 1) << 0) |
                     ((pixel[1] & 1) << 1) |
                     ((pixel[2] & 1) << 2);
            if (c == '\0') return message;
            message += c;
        }
    }
    return message;
}
