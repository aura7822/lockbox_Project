#include "utils.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>

std::string Utils::promptHidden(const std::string& msg) {
    std::cout << msg;
    termios oldt, newt;
    std::string pass;
    char c;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, pass);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << "\n";
    return pass;
}
