/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "vectorUtils.hpp"
std::string getSubVector(const std::string& vec, uint32_t start,
                            uint32_t size) {
  std::string ans(vec.begin() + start, vec.begin() + size + start);
  return ans;
}
