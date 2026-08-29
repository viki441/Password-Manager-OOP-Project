#pragma once
#include <vector>
#include <string>


std::vector<std::string> split(std::string commandLine, char symbol);

bool hasTxtExtension(const std::string& filename);
std::string removeQuotes(const std::string& input);

bool isAlphaAZ(const std::string& text);
//check password for lenght
//check password for 10 unique characters