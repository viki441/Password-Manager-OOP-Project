#pragma once
#include "Cipher.h"
#include <vector>
class TextCodeCipher : public Cipher
{
private:
	std::string configPath;
	std::vector<char> symbols; // raw
	std::vector<int> encoding; 

public:
	TextCodeCipher();
	TextCodeCipher(const TextCodeCipher& other);
	TextCodeCipher& operator=(const TextCodeCipher& other);

	TextCodeCipher(TextCodeCipher&& other) = delete;
	TextCodeCipher& operator=(TextCodeCipher&& other) = delete;

	virtual std::string getName() const override;
	virtual std::string encrypt(const std::string& text) const override;
	virtual std::string decrypt(const std::string& text) const override;
	virtual std::string getConfiguration() const override;

	virtual void readConfiguration() override;
	virtual void setConfiguration(const std::string& text) override;
	virtual void saveCipherInfo(std::ofstream& file) const override;
	virtual void loadCipherConfiguration(std::ifstream& file) override;

	virtual Cipher* clone() const override;

	virtual bool validByKey(const std::string& text) override;

	bool isSymbolInConfig(const char& c);

	~TextCodeCipher() override = default;
};