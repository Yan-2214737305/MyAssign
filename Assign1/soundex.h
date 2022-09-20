/**
 * File: soundex.h
 *
 * This is just here to communicate the prototype 
 * information for the functions in soundex.cpp that 
 * will be called from main.cpp
 */
#pragma once
#include <string>

void soundexSearch(std::string filepath);
std::string soundex(std::string s);
std::string removeNonLetters(std::string s);
int codeRules(std::string sonStr);
std::string soundexRecode(std::string s);
std::string mergeNum(std::string num);
std::string replaceCapital(std::string capitalLetter, std::string num);
std::string filterChar(std::string s, char drop);
std::string completeCode(std::string s, int remainLen);
