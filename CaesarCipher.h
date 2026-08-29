#pragma once
#include "Cipher.h"

class CaesarCipher : public Cipher
{
private:

	static constexpr int ASCII_START = 32;
	static constexpr int ASCII_END = 125;
	static constexpr int MOD = ASCII_END - ASCII_START + 1;


	int key;
	char shiftChar(char c, int shiftAmount) const;

public:
	CaesarCipher();
	CaesarCipher(const CaesarCipher& other);
	CaesarCipher& operator=(const CaesarCipher& other);

	CaesarCipher(CaesarCipher&& other) = delete;
	CaesarCipher& operator=(CaesarCipher&& other) = delete;

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

	~CaesarCipher() override;

};