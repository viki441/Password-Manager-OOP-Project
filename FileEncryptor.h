#pragma once
#include <string>

class FileEncryptor
{
public:
    static std::string encrypt(const std::string& text,
        const std::string& password);

    static std::string decrypt(const std::string& text,
        const std::string& password);
};