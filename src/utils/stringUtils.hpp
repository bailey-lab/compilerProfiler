#pragma once
/*
 * 
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "common.h"


template <typename T>
std::string leftPadNumStr(T num, T highestNumber = 10) {
  std::stringstream ans;
  if (num == 0) {
    ans << std::string(log10(highestNumber), '0');
  } else {
    ans << std::string(((int)log10(highestNumber) - (int)log10(num)), '0');
  }
  ans << num;
  return ans.str();
}

std::string getTimeFormat(double timeInSecondsOriginal, bool wordy,
                          int secondsDecimalPlaces);

void stringToUpper(std::string& str);
void stringToLower(std::string& str);
std::string stringToUpperReturn(std::string str);
std::string stringToLowerReturn(std::string str);
void changeStringVectorToLowerCase(VecStr& vec) ;
std::string get_cwd();
bool stringContainsAllDigits(const std::string& str);

template <typename T>
std::string vectorToString(const std::vector<T>& vectorToConvert,
                           const std::string& delim = " ") {
  if (vectorToConvert.empty()) {
    return "";
  }
  std::stringstream tempStringStream;
  copy(vectorToConvert.begin(), vectorToConvert.end(),
       std::ostream_iterator<T>(tempStringStream, delim.c_str()));
  std::string returnString = tempStringStream.str().c_str();
  returnString.erase(returnString.size() - (int)delim.size());
  return returnString;
}
std::vector<std::string> tokenizeString(const std::string& str, const std::string& delim,
                      bool addEmptyToEnd = false);

std::vector<char> processAlphStrVecChar(const std::string & alphabetStr, const std::string & delim);
std::pair<std::vector<char>, std::vector<uint32_t>> processAlphStrVecCharCounts(const std::string & alphabetStr,
		const std::string & delim);
