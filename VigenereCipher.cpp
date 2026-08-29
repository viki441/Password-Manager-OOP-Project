#include "VigenereCipher.h"
#include <cctype>

//char VigenereCipher::normalizeKeyChar(char c) const
//{
//    //2 loops
//
//    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
//        return c - 32;
//    return c;
//}

char VigenereCipher::encryptChar(char c, char k) const
{
    int shift = isupper(k) ? k - 'A' : k - 'a';

    if (isupper(c))
        return char((c - 'A' + shift) % 26 + 'A');
    else
        return char((c - 'a' + shift) % 26 + 'a');
}

char VigenereCipher::decryptChar(char c, char k) const
{
    int shift = isupper(k) ? k - 'A' : k - 'a';

    if (isupper(c))
        return char((c - 'A' - shift + 26) % 26 + 'A');
    else
        return char((c - 'a' - shift + 26) % 26 + 'a');
}


std::string VigenereCipher::encrypt(const std::string& text) const
{
    std::string result;
    int keyIndex = 0;

    for (char c : text)
    {
 
        if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
        {
            char k = key[keyIndex % key.size()];
            result += encryptChar(c, k);
            keyIndex++;
        }
        else
            result += c;       
    }
    return result;
}

std::string VigenereCipher::decrypt(const std::string& text) const
{
    std::string result;
    int keyIndex = 0;

    for (char c : text)
    {
        if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
        {
            char k = key[keyIndex % key.size()];
            result += decryptChar(c, k);
            keyIndex++;
        }
        else
            result += c;     
    }
    return result;
}


void VigenereCipher::setConfiguration(const std::string& text)
{
    key.clear();

    for (char c : text)
    {
        if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z'))
            throw std::invalid_argument("Invalid symbol is configuration for Vigenere!\n");
         //normalizeKeyChar(c);
        else
            key += c;
    }
    if (key.empty())
        throw std::invalid_argument("Vigenere key cannot be empty!\n");
}

std::string VigenereCipher::getConfiguration() const
{
    return key;
}

void VigenereCipher::readConfiguration()
{
    std::cout << "Enter Vigenere key (letters only): ";

    std::string input;
    std::getline(std::cin, input);

    setConfiguration(input);
}

void VigenereCipher::saveCipherInfo(std::ofstream& file) const
{
    file << getName() << '\n';
	file << key << '\n';
}

void VigenereCipher::loadCipherConfiguration(std::ifstream& file)
{
    std::getline(file, key);
    //file >> key;
}


std::string VigenereCipher::getName() const
{
    return "Vigenere";
}

Cipher* VigenereCipher::clone() const
{
    return new VigenereCipher(*this);
}



bool VigenereCipher::isSymbolInConfig(const char& c)
{
    for (char ch : key)
    {
        if (c - '\0' == ch - '\0') return true;
    }
    return false;

}

bool VigenereCipher::validByKey(const std::string& text)
{
  /*  for (char c : text)
    {
        if (!isSymbolInConfig(c)) return false;
    }*/
    return true;
}