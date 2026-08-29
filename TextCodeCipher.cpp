#include "TextCodeCipher.h"
#include "Utils.h"

TextCodeCipher::TextCodeCipher()
{
    configPath = "";
    symbols.clear();
    encoding.clear();
}
TextCodeCipher::TextCodeCipher(const TextCodeCipher& other)
{
    configPath = other.configPath;
    symbols = other.symbols;
    encoding = other.encoding;
}
TextCodeCipher& TextCodeCipher::operator=(const TextCodeCipher& other)
{
    if (this != &other)
    {
        configPath = other.configPath;
        symbols = other.symbols;
        encoding = other.encoding;
    }

    return *this;
}

std::string TextCodeCipher::encrypt(const std::string& text) const
{
    std::string result;

    for (char c : text)
    {
        if (c < 32 || c > 126 || encoding[c] == -1)
            throw std::invalid_argument("Invalid character for TextCode encryption!\n");

        result += std::to_string(encoding[c]);
        result += " ";
    }

    return result;
}
std::string TextCodeCipher::decrypt(const std::string& text) const
{
    std::string result;
    std::string token;

    for (size_t i = 0; i <= text.size(); i++)
    {
        if (i == text.size() || text[i] == ' ')
        {
            if (!token.empty())
            {
                int index = std::stoi(token);

                if (index < 0 || index >= (int)symbols.size())
                    throw std::invalid_argument("Invalid index for TextCode cipher decryption!\n");

                result += symbols[index];
                token.clear();
            }
        }
        else       
            token.push_back(text[i]);  
    }
    return result;
}



void TextCodeCipher::readConfiguration()
{
    std::cout << "Enter path to reference text file: ";
    std::getline(std::cin, configPath);

    std::ifstream file(removeQuotes(configPath));
    if (!file)
       throw std::invalid_argument("TextCode: Cannot open config file for TextCode cipher key!\n");

    //TO DO: require atleast 30 unique symbols

    std::string text;
    std::getline(file, text, '\0');

    setConfiguration(removeQuotes(text));
}



void TextCodeCipher::setConfiguration(const std::string& text)
{
    std::string validText = removeQuotes(text);

    //in case setConfiguration is accessed with path
    if (hasTxtExtension(validText))
    {
        std::ifstream file(validText);
        std::getline(file, validText, '\0');
        file.close();
    }

    symbols.clear();
    encoding.assign(153, -1);

    int index = 0;

    for (char c : validText)
    {
        if (c < 32 || c > 154)//all english letters and other symbols
            continue; //skips all others.

        if (encoding[(unsigned char)c] == -1)
        {
            encoding[(unsigned char)c] = index++;
            symbols.push_back(c);
        }
    }

   /* for (char c = 32; c <= 126; c++)
    {
        if (encoding[(unsigned char)c] == -1)
        {
            encoding[(unsigned char)c] = index++;
            symbols.push_back(c);
        }
    }*/
}

std::string TextCodeCipher::getConfiguration() const
{
    return configPath;
}
void TextCodeCipher::saveCipherInfo(std::ofstream& file) const
{
    file << getName() << "\n";
    file << removeQuotes(configPath) << "\n";
}

void TextCodeCipher::loadCipherConfiguration(std::ifstream& file)
{
    std::getline(file, configPath);

    std::ifstream f(configPath);
    if (!f)
        throw std::invalid_argument("Cannot load TextCode config file!\n");

    std::string text;
    std::getline(f, text, '\0');
    f.close();

    setConfiguration(text);
}


std::string TextCodeCipher::getName() const
{
    return "TextCode";
}
Cipher* TextCodeCipher::clone() const
{
    return new TextCodeCipher(*this);
}

bool TextCodeCipher::isSymbolInConfig(const char& c)
{
    for (char ch : symbols)
    {
        if (c - '\0' == ch - '\0') return true;
    }
    return false;

}

bool TextCodeCipher::validByKey(const std::string& text)
{
    for (char c : text)
    {
        if (!isSymbolInConfig(c)) return false;
    }
    return true;
}