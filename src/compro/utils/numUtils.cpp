/*
 *
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/utils/numUtils.hpp"

namespace compro {

std::vector<double> getRange(double start, double stop, uint32_t num) {
  double difference = stop - start;
  double step = difference / (num - 1);
  std::vector<double> ans;
  for (const auto &i : iter::range<uint32_t>(0, num)) {
    ans.emplace_back((i * step) + start);
  }
  return ans;
}

} // compro
