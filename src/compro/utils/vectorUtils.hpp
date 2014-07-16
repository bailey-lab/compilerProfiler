#pragma once
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"
#include "compro/utils/stringUtils.hpp"
namespace compro {
template <class T>
void addOtherVec(std::vector<T>& reads, const std::vector<T>& otherVec) {
  reads.reserve(reads.size() + otherVec.size());
  reads.insert(reads.end(), otherVec.begin(), otherVec.end());
}
template <typename T>
bool vectorContains(const std::vector<T>& vec, const T& search) {
  for (const auto& iter : vec) {
    if (iter == search) {
      return true;
    }
  }
  return false;
}

template <typename T>
std::vector<T> getSubVector(const std::vector<T>& vec, uint32_t start,
                            uint32_t size) {
  std::vector<T> ans(vec.begin() + start, vec.begin() + size + start);
  return ans;
}

std::string getSubVector(const std::string& vec, uint32_t start, uint32_t size);

template <typename T>
std::vector<T> convertStringToVector(const std::string& str,
                                     const std::string& delim) {
  std::vector<T> ans;
  VecStr toks = tokenizeString(str, delim);
  for (const auto& t : toks) {
    std::stringstream tempStream;
    tempStream << t;
    T tempObject;
    tempStream >> tempObject;
    ans.push_back(tempObject);
  }
  return ans;
}

} // compro

