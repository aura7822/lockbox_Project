// src/platform/pwd_win.cpp
#include "platform/pwd.h"
#include <iostream>
#include <conio.h>

std::string getPassword(const std::string &prompt) {
    std::string password;
    std::cout << prompt;
    std::cout.flush();

    char ch;
    while ((ch = _getch()) != '\r') { // Enter key
        if (ch == '\b') { // Backspace
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        } else {
            password.push_back(ch);
            std::cout << '*';
        }
    }

    std::cout << std::endl;
    return password;
}
