#include "CipherFactory.h"
#include "CaesarCipher.h"
#include "TextCodeCipher.h"
#include "HillCipher.h"
#include "VigenereCipher.h"



Cipher* CipherFactory::createCipher(const std::string& type)
{
    if (type == "Caesar")
    {
        return new CaesarCipher();
    }
    else if (type == "TextCode")
    {
        return new TextCodeCipher();
    }
    else if (type == "Hill")
    {
        return new HillCipher();
    }
    else if (type == "Vigenere" || type == "Vigenère")
    {
        return new VigenereCipher();
    }
  
    return nullptr;
}

