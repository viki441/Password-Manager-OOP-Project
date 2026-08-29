#include "CaesarCipher.h"



CaesarCipher::CaesarCipher()
	:key(0)
{}
CaesarCipher::CaesarCipher(const CaesarCipher& other)
	:key(other.key)
{}
CaesarCipher& CaesarCipher::operator=(const CaesarCipher& other)
{
	if (this != &other)
		key = other.key;
	
	return *this;
}

CaesarCipher::~CaesarCipher()
{}

char CaesarCipher::shiftChar(char c, int shift) const
{
	if (c < ASCII_START || c > ASCII_END)
		throw std::invalid_argument("Invalid ASCII character in CaesarCipher\n");

	int normalized = c - ASCII_START;
	int shifted = (normalized + shift) % MOD;

	if (shifted < 0)
		shifted += MOD;

	return static_cast<char>(shifted + ASCII_START);
}

std::string CaesarCipher::encrypt(const std::string& text) const
{
	std::string result;
	for (char c : text)
		result.push_back(shiftChar(c, key));

	return result;
}

std::string CaesarCipher::decrypt(const std::string& text) const
{
	std::string result;
	for (char c : text)
		result.push_back(shiftChar(c, -key));

	return result;
}


std::string CaesarCipher::getName() const
{
	return "Caesar";
}

std::string CaesarCipher::getConfiguration() const
{
	return std::to_string(key);
}

void CaesarCipher::setConfiguration(const std::string& text)
{
	key = stoi(text);
}


void CaesarCipher::readConfiguration()
{
	std::cout << "Enter cipher for Caesar: \n";
	while (true)
	{
		if (std::cin >> key)
		{
			std::cin.ignore(10000, '\n');
			break;
		}

		std::cin.clear();
		std::cin.ignore(10000, '\n');

		std::cout << "Invalid input. Enter a valid number: ";
	}
}

void CaesarCipher::saveCipherInfo(std::ofstream& file) const
{	
	file << getName() << '\n';
	file << key << '\n';
}

void CaesarCipher::loadCipherConfiguration(std::ifstream& file)
{
	file >> key;
	file.ignore(10000, '\n');
}

Cipher* CaesarCipher::clone() const
{
	return new CaesarCipher(*this);
}


bool CaesarCipher::validByKey(const std::string& text)
{
	return true;
}
