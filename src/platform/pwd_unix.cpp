// src/platform/pwd_unix.cpp
//#include "platform/pwd.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include "pwd.h"


std::string getPassword(const std::string &prompt) {
    std::string password;
    std::cout << prompt;
    std::cout.flush();

    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO; // disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // restore echo
    std::cout << std::endl;

    return password;
}
