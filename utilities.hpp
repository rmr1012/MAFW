#ifndef UTILITIES_H
#define UTILITIES_H
#include <cstring>
#include <string>
#include <vector>

std::vector<char> str2HexVec(std::string inStr, char deli);
bool strInStr(std::string needle,std::string haystack);
bool translateTruth(std::string inTruth);

std::string toUpper(std::string inStr);

#endif
