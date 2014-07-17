#pragma once
/*
 * profilerSetUp.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: nickhathaway
 */

#include "compro/common.h"
#include "compro/utils.h"
#include "compro/programUtils/programSetUp.hpp"

namespace compro {

class profilerSetUp : public programSetUp {
 public:
  profilerSetUp(int argc, char* argv[]) : programSetUp(argc, argv) {
    initializeDefaults();
  }
  profilerSetUp(const commandLineArguments& inputCommands)
      : programSetUp(inputCommands) {
    initializeDefaults();
  }
  profilerSetUp(const MapStrStr& inputCommands) : programSetUp(inputCommands) {
    initializeDefaults();
  }

  void initializeDefaults();

  // some defaults
  std::string logFileName_ = "";
  bool header_ = false;
  bool verbose_ = false;
  std::string extraInfoStr_ = "";
  std::vector<std::pair<std::string, std::string>> extraInfo_;

  // for logging, will print to logFile or to std::cout if no log file specified
  std::stringstream logging_;

  ~profilerSetUp() {
    if (logFileName_ != "") {
      std::ofstream logFile;
      // open file for appending if it already exists
      openTextFile(logFile, logFileName_, false, true, false);
      logFile << logging_.str();
    } else {
      std::cout << std::fixed << logging_.str();
    }
  }

 private:
  void processHeader();
  void processVerbose();
  void processExtra();
  void processLogFileName();
};

} /* namespace compro */
