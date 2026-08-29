#include "CommandHandler.h"
#include "PasswordFile.h"
#include "CipherFactory.h"
#include "Utils.h"
#include <iostream>
#include <stdexcept>


CommandHandler::CommandHandler()
{
}

CommandHandler::~CommandHandler()
{
    delete currentFile;
}

bool CommandHandler::isTextValid(const std::string& text)
{
    for (char c : text)
    {
        if (c < 32 || c > 126)
            throw std::invalid_argument("Text has unsupported symbol.\n");
    }
    return true;
}


void CommandHandler::execute(const std::string& commandLine, bool& isRunning)
{
    if (!isTextValid(commandLine))
        return;

    std::vector<std::string> tokens = split(commandLine, ' ');
    
    if (tokens.empty())
        return;

    if (tokens[0] == "create")
        create(tokens);

    else if (tokens[0] == "open")
        open(tokens);

    else if (tokens[0] == "close")
        closeFile();

    else if (tokens[0] == "save")
        save(tokens);

    else if (tokens[0] == "load")
        load(tokens);

    else if (tokens[0] == "update")
        update(tokens);

    else if (tokens[0] == "delete")
        remove(tokens);

    else if (tokens[0] == "list")
        list();

    else if (tokens[0] == "exit")
    {
        exitProgram();
        isRunning = false;
    }
    else
        throw std::invalid_argument("Invalid command!\n");
}

void CommandHandler::create(const std::vector<std::string>& args) 
{
    //for case create -> open -> create
    if (currentFile != nullptr)
        throw std::logic_error("Close the current file first!\n");
    
    if (args.size() != 4)
        throw std::out_of_range("Argument count not right!\n");


    std::string filename = args[1];
    std::string cipherName = args[2];
    std::string masterPassword = args[3];

    Cipher* cipher = CipherFactory::createCipher(cipherName);

    if (cipher == nullptr) 
        throw std::invalid_argument("Unknown cipher\n");


    cipher->readConfiguration();
    currentFile = new PasswordFile(filename, masterPassword, cipher);

    try
    {
        currentFile->create();
    }
    catch (const std::logic_error& e)
    {
        delete currentFile;
        currentFile = nullptr;
        throw;
    }
  
    //for case create -> create
    delete currentFile;
    currentFile = nullptr;
    std::cout << "\033[92mCREATED FILE SUCCESSFULLY!\033[0m\n";
}

void CommandHandler::open(const std::vector<std::string>& args)
{
  
    if (currentFile != nullptr)
        throw std::logic_error("Close the current OPENED file first or stop opening the alreagy opened file...!\n");

    if(args.size() != 3) 
        throw std::out_of_range("Argument count not right\n");
   
    std::string filepath = args[1];
    std::string userPassword = args[2]; //this must match  the decr. token

    std::ifstream file(filepath);
    if (!file)throw std::invalid_argument("error opening file or file didn't exist in the 1st place\n");

    std::string cipherName;
    std::getline(file, cipherName);

    Cipher* cipher = CipherFactory::createCipher(cipherName);
    if (cipher == nullptr) throw std::invalid_argument("Unknown cipher\n");

    cipher->loadCipherConfiguration(file);

    //here we take the encr. token FROM THE FILE
    std::string encrToken;
    std::getline(file, encrToken);
    std::string decrToken = FileEncryptor::decrypt(encrToken, userPassword);

    if (decrToken != PasswordFile::TOKEN)
    {
        std::cout << "Invalid password\n";
        delete cipher;
        return;
    }

    //if userPassword is defined correctly, it macthes masterpassword
    currentFile = new PasswordFile(filepath, userPassword, cipher);

    //at this point the pointer is at the beginning of the 1st record
    currentFile->loadFromStream(file);
    

    file.close();
    std::cout << "\033[92mFILE OPENED SUCCESSFULLY\033[0m\n";
}

void CommandHandler::closeFile()
{
    
    if (!currentFile)  
        throw std::logic_error("No file is currently opened, in order to be closed!\n");
    

    if (currentFile->isModified())
    {
        std::cout << "Do you want to save the changes? Y/N\n";
        char c;
        std::cin >> c;
        std::cin.ignore(10000, '\n');
        if (std::tolower(c) == 'y')
        {
            std::string filename = currentFile->getFilename();
            std::ofstream file(filename);
            currentFile->saveToStream(file);
            currentFile->setModified(false);
        }
    }
    delete currentFile;
    currentFile = nullptr;

    std::cout << "\033[92mCLOSED FILE SUCCESSFULLY!\033[0m\n";
}

//basically here "save" serves as adding new entries
void CommandHandler::save(const std::vector<std::string>& args)
{
    if (!currentFile)    
        throw std::logic_error("No file is currently opened, in order to be closed!\n");

    if (args.size() < 4 || args.size() > 5)
        throw std::out_of_range("Argument count not right\n");

    std::string website = args[1];
    std::string username = args[2];
    std::string password = args[3];
    std::string cipherName = PasswordFile::DEFAULT_CIPHER;
    std::string configuration = PasswordFile::DEFAULT_CIPHER;

    if (currentFile->doesEntryExists(website, username))
        throw std::logic_error("Website with this username already exists!\n");

    //additional cipher
    if (args.size() == 5)
    {
        cipherName = args[3];
        Cipher* templateCipher = CipherFactory::createCipher(cipherName);

        if (!templateCipher)
        {
            throw std::invalid_argument("Invalid cipher!\n"); 
            delete templateCipher;
        }

        password = args[4];
        std::cout << "Enter configuration for " << cipherName << " cipher:\n";
        templateCipher->readConfiguration();
        configuration = templateCipher->getConfiguration();

        //delete templateCipher??

        if (!templateCipher->validByKey(password))
        {
            throw std::invalid_argument("Invalid password for custom cipher, please enter valid password or choose another cipher.\n");
        }
    }      
    currentFile->addEntry(website, username, password, cipherName, configuration);
    std::cout << "\033[92mENTRY SAVED TO BUFFER SUCCESSFULLY!\033[0m\n"; 
}

void CommandHandler::load(const std::vector<std::string>& args)
{
    if (!currentFile)   
        throw std::logic_error("No file is currently opened, in order to be closed!\n");
    
    if (args.size() != 3 && args.size() != 2)   
        throw std::out_of_range("Argument count not right\n");
    
    if (args.size() == 3)
    { 
        std::string website = args[1];
        std::string username = args[2];
        PasswordEntry* ps = currentFile->findEntry(website, username);
        if (ps)
        {
            currentFile->printEntry(*ps);       
            return;
        }
        std::cout << "There is no such username/website!\n";
    }
    else
    {
        std::string website = args[1];
        if (currentFile->doesWebsiteEntryExists(website))
        {
            currentFile->printAllEntries(website);
            return;
        }
        std::cout << "\033[93mThere is no such website!\033[0m\n";
    }
    std::cout << "\033[92mLOADED SUCCESSFULLY!\033[0m\n";
}

void CommandHandler::update(const std::vector<std::string>& args)
{
    if (!currentFile)   
        throw std::logic_error("No file is currently opened, in order to be closed!\n");
    

    if (args.size() != 4)
        throw std::out_of_range("Argument count not right\n");

    std::string website = args[1];
    std::string username = args[2];
    std::string newPassword = args[3];

    PasswordEntry* entryForUpdate = currentFile->findEntry(website, username);

    if (!entryForUpdate)
    {
        std::cout << "There is no such entry, do you want to save it? Y/N\n";
        char c;
        std::cin >> c;
        std::cin.ignore(10000, '\n');
        if (std::tolower(c) == 'y')
        {
            currentFile->addEntry(website, username, newPassword, PasswordFile::DEFAULT_CIPHER, PasswordFile::DEFAULT_CIPHER);
            std::cout << "\033[92mENTRY UPDATED TO BUFFER SUCCESSFULLY!\033[0m\n";
        }
        else
            std::cout << "\033[92mUPDATE REJECTED SUCCESSFULLY!\033[0m\n";
        return;
    }
    currentFile->updatePassword(entryForUpdate, newPassword);
}

void CommandHandler::remove(const std::vector<std::string>& args)
{
    if (!currentFile)    
        throw std::logic_error("No file is currently opened, in order to be closed!\n");  

    if (args.size() < 2 || args.size() > 3)
        throw std::out_of_range("Argument count not right\n");

    std::string website = args[1];

    if (args.size() == 3)
    {
        std::string username = args[2];

        if (!currentFile->doesEntryExists(website, username))
        {
            std::cout << "Entry for deletion not found.\n";
            return;
        }
        currentFile->removeEntry(website, username);
    }
    else
        currentFile->removeEntries(website);
}

void CommandHandler::list()
{
    if (!currentFile)   
        throw std::logic_error("No file is currently opened, in order to be closed!\n");
    
    currentFile->listEntries();
}

void CommandHandler::exitProgram()
{
    if(currentFile)
        closeFile();
}






