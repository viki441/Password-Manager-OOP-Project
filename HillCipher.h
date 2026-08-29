#pragma once
#include "Cipher.h"
#include <vector>


class HillCipher : public Cipher
{
private:

	static constexpr int ASCII_START = 32;
	static constexpr int ASCII_END = 125;
	static constexpr int MOD = ASCII_END - ASCII_START + 1;


	std::vector<std::vector<int>> key;
	std::vector<std::vector<int>> inverseKey;
	int n;


private:
	std::vector<std::vector<int>> readMatrix(std::istream& in, int n);

public:
	HillCipher();
	HillCipher(const HillCipher& other);
	HillCipher& operator=(const HillCipher& other);

	HillCipher(HillCipher&& other) = delete;
	HillCipher& operator=(HillCipher&& other) = delete;

	virtual std::string getName() const override;
	virtual std::string encrypt(const std::string& text) const override;
	virtual std::string decrypt(const std::string& text) const override;
	virtual std::string getConfiguration() const override;

	virtual void readConfiguration() override;
	virtual void setConfiguration(const std::string& text) override;
	virtual void saveCipherInfo(std::ofstream& file) const override;
	virtual void loadCipherConfiguration(std::ifstream& file) override;

	virtual Cipher* clone() const override;

	~HillCipher() override = default;

	virtual bool validByKey(const std::string& text) override;


	// REQUIRED interface (used by system)
	//std::string encrypt(const std::string& text) const;
	//std::string decrypt(const std::string& text) const;

	// INTERNAL math
	std::string encryptBlock(const std::string& text) const;
	std::string decryptBlock(const std::string& text) const;

	// RECORD logic
	std::string encryptRecord(const std::string& record) const;
	std::string decryptRecord(const std::string& record) const;


};