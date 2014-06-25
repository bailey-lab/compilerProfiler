#pragma once
/*
 * 
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "common.h"

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
std::string getSubVector(const std::string& vec, uint32_t start,
                            uint32_t size);
