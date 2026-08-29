#pragma once
#include "Cipher.h"



class CipherFactory
{
public:
    static Cipher* createCipher(const std::string& name);

    
};