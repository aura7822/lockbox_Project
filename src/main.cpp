#include <iostream>
#include <string>
#include <termcolor/termcolor.hpp>
#include "encryptor.h"
#include "obfuscator.h"
#include "imgutils.h"

using namespace std;

void printBanner() {
    cout << termcolor::bright_cyan
         << "╔══════════════════════════════════════════════════════╗\n"
         << "║                                                      ║\n"
         << "║           ███╗   ██╗██╗   ██╗██╗  ██╗██████╗          ║\n"
         << "║           ████╗  ██║██║   ██║██║ ██╔╝██╔══██╗         ║\n"
         << "║           ██╔██╗ ██║██║   ██║█████╔╝ ██║  ██║         ║\n"
         << "║           ██║╚██╗██║██║   ██║██╔═██╗ ██║  ██║         ║\n"
         << "║           ██║ ╚████║╚██████╔╝██║  ██╗██████╔╝         ║\n"
         << "║           ╚═╝  ╚═══╝ ╚═════╝ ╚═╝  ╚═╝╚═════╝          ║\n"
         << "║                    LOCKBOX TERMINAL                   ║\n"
         << "╚══════════════════════════════════════════════════════╝\n"
         << termcolor::reset << endl;
}

void printMenu() {
    cout << termcolor::bright_yellow
         << "╔════════════════════════════════════════╗\n"
         << "║              MAIN  MENU                ║\n"
         << "╠════════════════════════════════════════╣\n"
         << "║  1. Encrypt a file                     ║\n"
         << "║  2. Decrypt a file                     ║\n"
         << "║  3. Obfuscate text                     ║\n"
         << "║  4. Deobfuscate text                   ║\n"
         << "║  5. Hide message in image              ║\n"
         << "║  6. Extract hidden message from image  ║\n"
         << "║  7. Exit                               ║\n"
         << "╚════════════════════════════════════════╝\n"
         << termcolor::reset;
}

int main() {
    Encryptor enc;
    Obfuscator obfObj;
    int choice;

    while (true) {
        system("clear");
        printBanner();
        printMenu();

        cout << termcolor::bright_white << "\nChoose an option: " << termcolor::reset;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string input, output, password;
            cout << termcolor::cyan << "Enter path to file or folder to encrypt: " << termcolor::reset;
            getline(cin, input);
            cout << termcolor::cyan << "Enter output path: " << termcolor::reset;
            getline(cin, output);
            cout << termcolor::cyan << "Enter encryption password: " << termcolor::reset;
            getline(cin, password);

            if (enc.encryptFile(input, output, password))
                cout << termcolor::green << "✅ Encryption successful." << termcolor::reset << endl;
            else
                cout << termcolor::red << "❌ Encryption failed." << termcolor::reset << endl;
        }

        else if (choice == 2) {
            string input, output, password;
            cout << termcolor::cyan << "Enter path to encrypted file: " << termcolor::reset;
            getline(cin, input);
            cout << termcolor::cyan << "Enter output path: " << termcolor::reset;
            getline(cin, output);
            cout << termcolor::cyan << "Enter decryption password: " << termcolor::reset;
            getline(cin, password);

            if (enc.decryptFile(input, output, password))
                cout << termcolor::green << "✅ Decryption successful." << termcolor::reset << endl;
            else
                cout << termcolor::red << "❌ Decryption failed." << termcolor::reset << endl;
        }

        else if (choice == 3) {
            string text;
            cout << termcolor::cyan << "Enter text to obfuscate: " << termcolor::reset;
            getline(cin, text);

            string obf = obfObj.obfuscateText(text);
            cout << termcolor::green << "Obfuscated text: " << obf << termcolor::reset << endl;
        }

        else if (choice == 4) {
            string obf;
            cout << termcolor::cyan << "Enter obfuscated text: " << termcolor::reset;
            getline(cin, obf);

            string plain = obfObj.deobfuscateText(obf);
            cout << termcolor::green << "Deobfuscated text: " << plain << termcolor::reset << endl;
        }

        else if (choice == 5) {
            string image, message, out;
            cout << termcolor::cyan << "Enter path to input image: " << termcolor::reset;
            getline(cin, image);
            cout << termcolor::cyan << "Enter message to hide: " << termcolor::reset;
            getline(cin, message);
            cout << termcolor::cyan << "Enter output image path: " << termcolor::reset;
            getline(cin, out);

            if (ImgUtils::hideMessage(image, message, out))
                cout << termcolor::green << "✅ Message hidden successfully." << termcolor::reset << endl;
            else
                cout << termcolor::red << "❌ Failed to hide message." << termcolor::reset << endl;
        }

        else if (choice == 6) {
            string image;
            cout << termcolor::cyan << "Enter path to image: " << termcolor::reset;
            getline(cin, image);

            string extracted = ImgUtils::extractMessage(image);
            if (!extracted.empty())
                cout << termcolor::green << "Extracted message: " << extracted << termcolor::reset << endl;
            else
                cout << termcolor::red << "❌ No hidden message found." << termcolor::reset << endl;
        }

        else if (choice == 7) {
            cout << termcolor::bright_magenta << "\nExiting LockBox... Stay encrypted, stay safe.\n" << termcolor::reset;
            break;
        }

        else {
            cout << termcolor::red << "❌ Invalid option, try again." << termcolor::reset << endl;
        }

        cout << termcolor::bright_white << "\nPress Enter to continue...";
        cin.get();
    }

    return 0;
}
