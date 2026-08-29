#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

class Cipher
{
public:
	virtual std::string encrypt(const std::string& text) const = 0;
	virtual std::string decrypt(const std::string& text) const = 0;
	virtual std::string getName() const = 0;
	virtual Cipher* clone() const = 0;

	virtual std::string getConfiguration() const = 0;
	virtual void setConfiguration(const std::string& text) = 0;
	virtual void readConfiguration() = 0; 

	virtual void saveCipherInfo(std::ofstream& file) const = 0;
	virtual void loadCipherConfiguration(std::ifstream& file) = 0;
	virtual bool validByKey(const std::string& text) = 0;

	virtual ~Cipher() = default;
};