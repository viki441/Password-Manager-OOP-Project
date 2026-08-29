#pragma once

#include "FileEncryptor.h"
#include "PasswordEntry.h"
#include "Cipher.h"
#include <vector>

class PasswordFile
{

private:
    static const int PAGE = 10;
    Cipher* cipher;
    std::string filename;
    std::string masterPassword;
    

    std::vector<PasswordEntry> entries;


    bool modified;

    

public:
    static const std::string DEFAULT_CIPHER;
    static const std::string TOKEN;


    PasswordFile(const std::string& f, const std::string& mp, Cipher* c);
    ~PasswordFile();

    PasswordFile(const PasswordFile& other) = delete;
    PasswordFile& operator=(const PasswordFile& other) = delete;

    PasswordFile(PasswordFile&& other) = delete;
    PasswordFile& operator=(PasswordFile&& other) = delete;

    void create();
    void addEntry(const std::string& website, const std::string& username, const std::string& password, const std::string& name, const std::string& config);
    void updatePassword(PasswordEntry* entryForUpdate, const std::string& newPassword);

    void removeEntry(const std::string& website, const std::string& username);
    void removeEntries(const std::string& website);

    void listEntries() const;

    void saveToStream(std::ofstream& file);
    void loadFromStream(std::ifstream& file);
    void applyCustomCipherDecrypt(PasswordEntry& entry);
    void applyCustomCipherEncrypt(PasswordEntry& entry);
    void addHeader(std::ofstream& file);

    bool isModified() const;
    void setModified(bool m);

    unsigned getEntriesSize() const;

    std::string getFilename() const;

    bool doesEntryExists(const std::string& website, const std::string& username);
    bool doesWebsiteEntryExists(const std::string& website);

    PasswordEntry* findEntry(const std::string& website, const std::string& username);

    PasswordEntry parseEntry(const std::string& line);
    std::string serializeEntry(const PasswordEntry& e);



    void printEntry(const PasswordEntry& ps) const;
    void printAllEntries(const std::string& website) const;

};