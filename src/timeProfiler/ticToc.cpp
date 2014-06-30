
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "ticToc.hpp"
#include "utils/stringUtils.hpp"
namespace compro{
double TicToc::getRunTime() {
	return elapsed<zi::in_usecs>() / 1000000.0;
}
std::string TicToc::getStringRunTime(bool wordy, int decPlaces) {
  return getTimeFormat(getRunTime(), true, decPlaces);
}

void TicToc::print(const std::string& pre, std::ostream& out,
                   uint32_t indentAmount, int decPlaces) {
  out << std::string(indentAmount, '\t');
  if (pre != "") {
    out << pre << " ";
  }
  out << "(" << getStringRunTime(true, decPlaces) << ")" << std::endl;
}
}
