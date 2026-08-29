#include "Utils.h"

std::vector<std::string> split(std::string commandLine, char symbol)
{
	std::vector<std::string> result;
	bool inQuotes = false;
	std::string currentWord;

	for (size_t i = 0; i < commandLine.size(); i++)
	{
		char c = commandLine[i];

		if (c == '"') 
			inQuotes = !inQuotes;

		else if (c == symbol && !inQuotes)
		{
			if (!currentWord.empty())
			{
				result.push_back(currentWord);
				currentWord.clear();
			}
		}
		else
			currentWord += c;
		
	}
	if (!currentWord.empty())
		result.push_back(currentWord);

	return result;
}


bool hasTxtExtension(const std::string& filename)
{
	size_t dot = filename.find_last_of('.');
	size_t slash = filename.find_last_of("\\/");

	if (dot == std::string::npos)
		return false;

	if (slash != std::string::npos && dot < slash)
		return false;

	std::string ext = filename.substr(dot + 1); 

	return ext == "txt";
}


std::string removeQuotes(const std::string& input)
{
	if (input.size() >= 2 &&
		((input.front() == '"' && input.back() == '"') ||
			(input.front() == '\'' && input.back() == '\'')))
	{
		return input.substr(1, input.size() - 2);
	}

	return input;
}

bool isAlphaAZ(const std::string& text)
{
	for (char c : text)
	{

		if (c >= 'A' && c <= 'Z')
			continue;

		return false;
	}

	return true;
}