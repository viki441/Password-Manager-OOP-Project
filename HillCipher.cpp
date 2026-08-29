#include "HillCipher.h"
#include "Utils.h"

HillCipher::HillCipher() : n(0) {}
HillCipher::HillCipher(const HillCipher& other)
    : key(other.key),
    inverseKey(other.inverseKey),
    n(other.n)
{
}
HillCipher& HillCipher::operator=(const HillCipher& other)
{
    if (this != &other)
    {
        key = other.key;
        inverseKey = other.inverseKey;
        n = other.n;
    }

    return *this;
}

void HillCipher::readConfiguration()
{
    std::cout << "Matrix size: ";
    std::cin >> n;

    key.assign(n, std::vector<int>(n));
    inverseKey.assign(n, std::vector<int>(n));

    std::cout << "Enter KEY matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cin >> key[i][j];
            key[i][j] %= MOD;
        }
    }

    std::cout << "Enter INVERSE matrix:\n";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            std::cin >> inverseKey[i][j];
            inverseKey[i][j] %= MOD;
        }
    }

    std::cin.ignore(10000, '\n');
}
std::string HillCipher::encrypt(const std::string& text) const
{
    return encryptRecord(text);
}
std::string HillCipher::decrypt(const std::string& text) const
{
    return decryptRecord(text);
}
//std::string HillCipher::encrypt(const std::string& text) const
//{
//    std::string result;
//    std::vector<int> block(n);
//
//    for (size_t i = 0; i < text.size(); i += n)
//    {
//        for (int j = 0; j < n; j++)
//        {
//            if (i + j < text.size())
//            {
//                char c = text[i + j];
//
//                if (c < 'A' || c > 'Z')
//                    throw std::invalid_argument("Hill cipher supports only English letters.");
//
//                block[j] = c - 'A';
//            }
//            else            
//                block[j] = 0;          
//        }
//
//        for (int col = 0; col < n; col++)
//        {
//            int sum = 0;
//            for (int row = 0; row < n; row++)
//                sum += block[row] * key[row][col];
//
//            result += static_cast<char>((sum % 26) + 'A');
//        }
//    }
//
//    return result;
//}
//
//std::string HillCipher::decrypt(const std::string& text) const
//{
//    std::string result;
//    std::vector<int> block(n);
//
//    for (size_t i = 0; i < text.size(); i += n)
//    {
//        for (int j = 0; j < n; j++)
//        {
//            if (i + j >= text.size())
//                throw std::invalid_argument("Invalid Hill cipher text.");
//
//            char c = text[i + j];
//
//            if (c < 'A' || c > 'Z')
//                throw std::invalid_argument("Invalid Hill cipher text.");
//
//            block[j] = c - 'A';
//        }
//
//        for (int col = 0; col < n; col++)
//        {
//            int sum = 0;
//            for (int row = 0; row < n; row++)
//                sum += block[row] * inverseKey[row][col];
//
//            sum %= 26;
//            if (sum < 0)
//                sum += 26;
//
//            result += static_cast<char>(sum + 'A');
//        }
//    }
//
//    return result;
//}


std::string HillCipher::getName() const
{
    return "Hill";
}

std::string HillCipher::getConfiguration() const
{
    std::string result;

    result += std::to_string(n);
    result += "|";

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result += std::to_string(key[i][j]);

            if (!(i == n - 1 && j == n - 1))
                result += " ";
        }
    }

    result += "|";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result += std::to_string(inverseKey[i][j]);

            if (!(i == n - 1 && j == n - 1))
                result += " ";
        }
    }

    return result;
}



void HillCipher::setConfiguration(const std::string& text)
{
    std::vector<std::string> parts = split(text, '|');

    if (parts.size() != 3)
        throw std::invalid_argument("Invalid Hill configuration!\n");

    n = std::stoi(parts[0]);

    key.assign(n, std::vector<int>(n));
    inverseKey.assign(n, std::vector<int>(n));

    std::vector<std::string> keyValues = split(parts[1], ' ');
    std::vector<std::string> invValues = split(parts[2], ' ');

    if (keyValues.size() != n * n || invValues.size() != n * n)
        throw std::invalid_argument("Invalid matrix size!\n");

    int index = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)       
            key[i][j] = std::stoi(keyValues[index++]) % MOD;
        
    }

    index = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)        
            inverseKey[i][j] = std::stoi(invValues[index++]) % MOD;
        
    }
}

void HillCipher::saveCipherInfo(std::ofstream& file) const
{
    file << getName() << '\n';
    file << getConfiguration() << "\n";
}

void HillCipher::loadCipherConfiguration(std::ifstream& file)
{
    std::string configuration;
    std::getline(file, configuration);

    if (configuration.empty())
        std::getline(file, configuration);

    setConfiguration(configuration);
}

Cipher* HillCipher::clone() const
{
    return new HillCipher(*this);
}



std::vector<std::vector<int>> HillCipher::readMatrix(std::istream& in, int n)
{
    std::vector<std::vector<int>> m(n, std::vector<int>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (!(in >> m[i][j]))
                throw std::invalid_argument("Invalid matrix input for Hill cipher!\n");

            m[i][j] %= MOD;

            if (m[i][j] < 0)
                m[i][j] += MOD;
        }
    }
    return m;
}





std::string HillCipher::encryptRecord(const std::string& record) const
{
    return encryptBlock(record);
}
std::string HillCipher::decryptRecord(const std::string& record) const
{
    return decryptBlock(record);
}


bool HillCipher::validByKey(const std::string& text)
{
    if (text.size() % n != 0)
        return false;

    return true;
}

std::string HillCipher::encryptBlock(const std::string& text) const
{

    std::string result;
    std::vector<int> block(n);

    for (size_t i = 0; i < text.size(); i += n)
    {
        for (int j = 0; j < n; j++)
        {
            if (i + j < text.size())
            {
                unsigned char c = text[i + j];

                if (c < ASCII_START || c > ASCII_END)
                    throw std::invalid_argument("Invalid character.\n");

                block[j] = c - ASCII_START;
            }
            else           
                block[j] = 0;   
        }

        for (int col = 0; col < n; col++)
        {
            int sum = 0;
            for (int row = 0; row < n; row++)
                sum += block[row] * key[row][col];

            sum %= MOD;

            result += static_cast<char>(sum  + ASCII_START);
        }
    }

    return result;
}
std::string HillCipher::decryptBlock(const std::string& text) const
{
    std::string result;
    std::vector<int> block(n);

    if(text.size() % n != 0)  throw std::invalid_argument("Invalid Hill text lenght!\n");

    for (size_t i = 0; i < text.size(); i += n)
    {
        for (int j = 0; j < n; j++)
        {

            unsigned char c = text[i + j];

            if (c < ASCII_START || c > ASCII_END)
                throw std::invalid_argument("Invalid Hill text.\n");

            block[j] = c - ASCII_START;
        }

        for (int col = 0; col < n; col++)
        {
            int sum = 0;
            for (int row = 0; row < n; row++)
                sum += block[row] * inverseKey[row][col];

            sum %= MOD;
            if (sum < 0) sum += MOD;

            result += static_cast<char>(sum + ASCII_START);
        }
    }

    return result;
}


