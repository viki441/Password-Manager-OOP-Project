#include "CommandHandler.h"
#include <iostream>
#include <stdexcept>

#include <windows.h>

void printRules()
{
    std::cout << "Command syntax:\n";
    std::cout << "create <filename> <cipher> <password>\n";
    std::cout << "open <filename> <password>\n";
    std::cout << "close\n";
    std::cout << "save <website> <user> [<cipher> <password>]\n";
    std::cout << "load <website> [<user>]\n";
    std::cout << "update <website> <user> <new-password>\n";
    std::cout << "delete <website> [<user>]\n";
    std::cout << "list\n";
    std::cout << "exit\n";

    std::cout << "\033[94mCaution! Anything requiring whitespace must be in double quotes!\033[0m\n";
    std::cout << "Next Command: \n";
}

int main()
{
    SetConsoleCP(CP_UTF8); //find alternative

    std::cout << "PASSWORD MANAGER\n";
    
    bool running = true;

    CommandHandler c;

    while (running)
    {
        printRules();
        try
        { 
            std::string input;
            std::getline(std::cin, input);
            c.execute(input, running);
        }
        catch (const std::exception& e)
        {
            std::cout << "\033[93mWARNING: " << e.what() << "\033[0m\n";
        }
       
    }
}
