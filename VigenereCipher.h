#pragma once
#include "Cipher.h"
#include <string>

class VigenereCipher : public Cipher
{
private:
    std::string key;

    char encryptChar(char c, char k) const;
    char decryptChar(char c, char k) const;

    //char normalizeKeyChar(char c) const;

public:
    VigenereCipher() = default;

    std::string encrypt(const std::string& text) const override;
    std::string decrypt(const std::string& text) const override;

    std::string getName() const override;
    Cipher* clone() const override;

    std::string getConfiguration() const override;
    void setConfiguration(const std::string& text) override;

    void readConfiguration() override;
    void saveCipherInfo(std::ofstream& file) const override;
    void loadCipherConfiguration(std::ifstream& file) override;

    ~VigenereCipher() override = default;


    bool validByKey(const std::string& text);
    bool isSymbolInConfig(const char& c);


};
