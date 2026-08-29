#include "PasswordFile.h"
#include "CipherFactory.h"

#include "Utils.h"
#include <fstream>
#include <iostream>

const std::string PasswordFile::TOKEN = "VALID";
const std::string PasswordFile::DEFAULT_CIPHER = "DEFAULT";

PasswordFile::PasswordFile(const std::string& f, const std::string& mp, Cipher* c)
	: cipher(nullptr), modified(false)
{
	filename = f;
	masterPassword = mp;
	cipher = c;
}
PasswordFile::~PasswordFile()
{
	delete cipher;
}

std::string PasswordFile::getFilename() const
{
	return filename;
}


void PasswordFile::setModified(bool m)
{
	modified = m;
}
bool PasswordFile::isModified() const
{
	return modified;
}

unsigned PasswordFile::getEntriesSize() const
{
	return entries.size();
}

bool PasswordFile::doesEntryExists(const std::string& website, const std::string& username)
{
	for (const PasswordEntry& entry : entries)
	{
		if (entry.website == website && entry.username == username)
			return true;
		
	}
	return false;
}
bool PasswordFile::doesWebsiteEntryExists(const std::string& website)
{
	for (const PasswordEntry& entry : entries)
	{
		if(entry.website == website)		
			return true;
		
	}
	return false;
}


PasswordEntry* PasswordFile::findEntry(const std::string& website, const std::string& username)
{
	for (PasswordEntry& entry : entries)
	{
		if (entry.website == website && entry.username == username)		
			return &entry;
		
	}

	return nullptr;
}

void PasswordFile::create()
{
	//check if file name is taken
	std::ifstream check(filename);

	if (check)
		throw std::logic_error("File already exists!\n");
	check.close();

	//check for .txt
	if(!hasTxtExtension(filename))
		throw std::logic_error("File extention invalid!\n");

	std::ofstream file(filename);
	if(!file) 
		throw std::invalid_argument("Error opening file!\n");

	// name -> config -> TOKEN ENCR. -- za ulesnenie na povecheto loadCipherInfo()
	addHeader(file);
	
	file.close();
}
void PasswordFile::updatePassword(PasswordEntry* entryForUpdate, const std::string& newPassword)
{
	if (cipher->validByKey(newPassword) == false)
	{
		throw std::invalid_argument("New password is not according to the Cipher rules! Try again, with a valid new password!\n");
	}
	if (entryForUpdate->password == newPassword)
	{
		std::cout << "New password is the same as the old one.\n";
		return;
	}

	//it is important to say again, we encrypt only when putting in the file.
	entryForUpdate->password = newPassword;
	modified = true;

	std::cout << "\033[92mPASSWORD WAS UPDATED SUCCESSFULLY\033[0m\n";

}



void PasswordFile::removeEntry(const std::string& website, const std::string& username)
{
	for (size_t i = 0; i < entries.size(); ++i)
	{
		if (entries[i].website == website && entries[i].username == username)
		{
			entries.erase(entries.begin() + i);

			std::cout << "\033[92mEntry deleted!\033[0m\n";
			modified = true;
			return;
		}
	}

	std::cout << "Entry not found!\n";
}
void PasswordFile::removeEntries(const std::string& website)
{
	bool removed = false;

	for (size_t i = 0; i < entries.size(); )
	{
		if (entries[i].website == website)
		{
			entries.erase(entries.begin() + i);
			removed = true;
		}
		else
			++i;
		
	}
	if (removed)
	{
		std::cout << "\033[92mEntries deleted!\033[0m\n";
		modified = true;
	}
	else
		std::cout << "\033[92mNo entries found!\033[0m\n";
}

void PasswordFile::addEntry(const std::string& website, const std::string& username, const std::string& password, const std::string& name, const std::string& config)
{

	if (!cipher->validByKey(password))
	{
		throw std::invalid_argument("Wrong format for password. Please enter password according to the rules of the selected cipher!\n");
	}

	//guarantees no duplicates, for now in: save(), update()...
	
	PasswordEntry entry;
	entry.website = website;
	entry.username = username;
	entry.password = password;
	entry.cipherName = name;
	entry.configuration = config;


	////problema na Hill: 
	//if (!isAlphaAZ(password) && cipher->getName() == "Hill")
	//	throw std::invalid_argument("Password for Hill cipher application must be only A-Z (UPPERCASE)\n!");
	//

	entries.push_back(entry);
	modified = true;
}

void PasswordFile::listEntries() const
{
	if (entries.empty())
		std::cout << "\033[92mNo saved entries\033[0m\n";

	std::cout << "\n\033[92mListing entries: \033[0m\n";
	int counter = 0;
	for (const PasswordEntry& entry : entries)
	{
		if (counter >= PAGE)
		{
			std::cout << "\033[94mPage is full, do you want to continue listing? Type Y for yes.\033[0m\n";
			char c;
			std::cin >> c;
			std::cin.ignore(10000, '\n');
			if (std::tolower(c) != 'y')
				return;		
			counter = 0;
		}
		std::cout << "\033[92mEntry: " << entry.website << " | " << entry.username << "\033[0m" << std::endl;
	}
}


//----------------------------CONSOLE RELATED
void PasswordFile::printEntry(const PasswordEntry& ps) const
{
	std::cout << "\033[92mENTRY:\n";
	std::cout << "Website: " << ps.website << std::endl;
	std::cout << "Username: " << ps.username << std::endl;
	std::cout << "Password: " << ps.password << std::endl;
	std::cout << "Cipher name: " << ps.cipherName << std::endl;
	std::cout << "Cipher configuration: " << ps.configuration << "\033[0m" << std::endl << std::endl;

}
void PasswordFile::printAllEntries(const std::string& website) const
{
	std::cout << "\nPrinting ALL ENTRIES with website " << website << std::endl;
	for (const PasswordEntry& entry : entries)
	{
		if (entry.website == website) 
			printEntry(entry);
	}
}


//--------------------------FILE RELATED

void PasswordFile::applyCustomCipherEncrypt(PasswordEntry& entry)
{
	std::string additionalCipher = entry.cipherName;
	std::string additionalConfig = entry.configuration;

	/*if (cipher->getName() == "Hill" && additionalCipher == DEFAULT_CIPHER)
	{
		additionalCipher = cipher->getName();
		additionalConfig = cipher->getConfiguration();
	}*/
		
	Cipher* c = CipherFactory::createCipher(additionalCipher);
	//c->readConfiguration();
	c->setConfiguration(additionalConfig);

	entry.password = c->encrypt(entry.password);

	delete c;
}
void PasswordFile::applyCustomCipherDecrypt(PasswordEntry& entry)
{
	std::string additionalCipher = entry.cipherName;
	std::string additionalConfig = entry.configuration;

	if (cipher->getName() == "Hill" && additionalCipher == DEFAULT_CIPHER)
	{
		additionalCipher = cipher->getName();
		additionalConfig = cipher->getConfiguration();
	}


	Cipher* c = CipherFactory::createCipher(additionalCipher);
	c->setConfiguration(additionalConfig);

	entry.password = c->decrypt(entry.password);

	delete c;
}

void PasswordFile::loadFromStream(std::ifstream& file)
{
	entries.clear();
	std::string line;

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;


		std::string decrypted = cipher->decrypt(line);
		PasswordEntry tempPE = parseEntry(decrypted);

		if (tempPE.cipherName != DEFAULT_CIPHER )//|| cipher->getName() == "Hill" why was this her5e?
		{
			applyCustomCipherDecrypt(tempPE);
			entries.push_back(tempPE);
			continue;
		}

		entries.push_back(tempPE);
	}
}
void PasswordFile::saveToStream(std::ofstream& file)
{
	file.clear();
	addHeader(file);

	for (PasswordEntry& entry : entries)
	{
		if (entry.cipherName != DEFAULT_CIPHER)
		{
			applyCustomCipherEncrypt(entry);
			if (cipher->validByKey(entry.password) == false)
			{

				std::cout << "\033[93mAfter the custom encryption, " 
				<< entry.username << " " << entry.website << entry.password 
				<< " is invalid for default encryption.\nEntry will NOT be saved due to user error.\033[0m\n";
				continue;
			}
			std::string serialized = serializeEntry(entry);
			file << cipher->encrypt(serialized) << "\n";
			continue;
		}

		std::string serialized = serializeEntry(entry);
		file << cipher->encrypt(serialized) << "\n";
	}
	modified = false;
}

void PasswordFile::addHeader(std::ofstream& file)
{
	cipher->saveCipherInfo(file);
	file << FileEncryptor::encrypt(TOKEN, masterPassword) << "\n";
}


PasswordEntry PasswordFile::parseEntry(const std::string& line)
{
	std::vector<std::string> splited = split(line, '|');

	if (splited.size() < 3)
		throw std::runtime_error("Corrupted entry!\n");

	PasswordEntry entry;

	entry.website = splited[0];
	entry.username = splited[1];
	entry.password = splited[2];

	if (splited.size() > 3)
		entry.cipherName = splited[3];

	if (splited.size() > 4)
		entry.configuration = splited[4];

	if (splited.size() > 4 && entry.cipherName == "Hill")
		entry.configuration = splited[4] + '|' + splited[5] + '|' + splited[6];


	return entry;
}

std::string PasswordFile::serializeEntry(const PasswordEntry& e)
{
	return e.website + "|" + e.username + "|" + e.password + "|" +
		e.cipherName + "|" + e.configuration;
}
















