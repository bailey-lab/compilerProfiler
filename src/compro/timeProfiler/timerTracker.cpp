
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/timeProfiler/timerTracker.hpp"
#include "compro/utils/stringUtils.hpp"
namespace compro {

double timeTracker::getRunTime() const {

  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::high_resolution_clock::now() - start_).count() /
         static_cast<double>(std::chrono::high_resolution_clock::period::den);
}


std::string timeTracker::getStringRunTime(bool wordy, int decPlaces) const {
  return getTimeFormat(getRunTime(), true, decPlaces);
}

void timeTracker::print(const std::string& pre, std::ostream& out,
                        uint32_t indentAmount, int decPlaces) const {
  out << std::string(indentAmount, '\t');
  if (pre != "") {
    out << pre << " ";
  }
  out << "(" << getStringRunTime(true, decPlaces) << ")" << std::endl;
}

void timeTracker::reset() {
  start_ = std::chrono::high_resolution_clock::now();
}
}
