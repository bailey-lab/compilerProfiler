/*
 * compilerInfo.cpp
 *
 *  Created on: Jun 29, 2014
 *      Author: nickhathaway
 */

#include "compro/getInfo/compilerInfo.hpp"
#include "compro/timeProfiler/timerTracker.hpp"
namespace compro {
/*
 * get compiler constants
 */
#if defined(__clang__)
std::string getCompilerVersion() {
  std::string versionStr = __VERSION__;
  if (versionStr.find("Apple") != std::string::npos) {
    size_t llvmPos = versionStr.find("LLVM");
    size_t numPos = versionStr.find_first_of("0123456789", llvmPos);
    size_t notNumPos = versionStr.find_first_not_of("0123456789.", numPos);
    std::string strVerNum = versionStr.substr(numPos, notNumPos - numPos);
    return "apple_" + strVerNum;
  } else {
    size_t llvmPos = versionStr.find("Clang");
    size_t numPos = versionStr.find_first_of("0123456789", llvmPos);
    size_t notNumPos = versionStr.find_first_not_of("0123456789.", numPos);
    std::string strVerNum = versionStr.substr(numPos, notNumPos - numPos);
    return strVerNum;
  }
}
#elif defined(__GNUC__) || defined(__GNUG__)
std::string getCompilerVersion() { return __VERSION__; }
#else
std::string getCompilerVersion() { return __VERSION__; }
#endif

std::string getRunInfo(
    const std::string& delim, bool getHeader,
    const std::vector<std::pair<std::string, std::string>>& extraInfo,
    const timeTracker& timmer) {
  std::stringstream tempStream;
  tempStream << std::fixed;
  if (getHeader) {
    for (const auto& extra : extraInfo) {
      tempStream << extra.first << delim;
    }
    tempStream << "compiler" << delim << "version" << delim << "optLevel"
               << delim << "funrollLoops" << delim << "operatingSystem";
    tempStream << delim << "time";
  } else {
    for (const auto& extra : extraInfo) {
      tempStream << extra.second << delim;
    }
    tempStream << compilerUsed << delim << getCompilerVersion() << delim
               << optimizationLevel << delim << unrollLoopsUsed << delim
               << operatingSystem;

    tempStream << delim << timmer.getRunTime();
  }
  return tempStream.str();
}
}
