#include "FileEncryptor.h"

std::string FileEncryptor::encrypt(const std::string& text,
    const std::string& password)
{
    std::string result;

    for (size_t i = 0; i < text.size(); i++)
    {
        char p = text[i];
        char k = password[i % password.size()];

        char enc = ((p - 32) + (k - 32)) % 95 + 32;
        result.push_back(enc);
    }

    return result;
}

std::string FileEncryptor::decrypt(const std::string& text,
    const std::string& password)
{
    std::string result;

    for (size_t i = 0; i < text.size(); i++)
    {
        char c = text[i];
        char k = password[i % password.size()];

        char dec = ((c - 32) - (k - 32) + 95) % 95 + 32;
        result.push_back(dec);
    }

    return result;
}