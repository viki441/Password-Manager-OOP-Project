#pragma once
#include <vector>
#include <string>

class PasswordFile;
class CommandHandler
{
private:

    PasswordFile* currentFile = nullptr;
    bool isTextValid(const std::string& text);
  
private:

    void create(const std::vector<std::string>& args);
    void open(const std::vector<std::string>& args);
    void closeFile();

    void save(const std::vector<std::string>& args);
    void load(const std::vector<std::string>& args);
    void update(const std::vector<std::string>& args);
    void remove(const std::vector<std::string>& args);

    void list();
    void exitProgram();

public:
    CommandHandler();
    ~CommandHandler();

    void execute(const std::string& commandLine, bool& isRunning);
};