#pragma once
/*
 * 
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "common.h"
#include "programUtils/commandLineArguments.hpp"
#include "programUtils/parameter.hpp"
#include "timeProfiler/ticToc.hpp"
#include "utils/ansiUtils.hpp"
namespace compro{
class programSetUp {

 public:
  // constructors
  programSetUp(int argc, char *argv[]);
  programSetUp(const commandLineArguments &inputCommands);
  programSetUp(const MapStrStr &inputCommands);
  // map to hold the commands
  commandLineArguments commands_;

  TicToc timer_;
  // valid options
  VecStr validOptions_;
  VecStr warnings_;
  bool failed_ = false;
  // for outputting
  uint32_t width_ = 80;
  uint32_t indent_ = 4;
  // parameters for logging
  parametersHolder pars_;

  //void writeParametersFile(const std::string &fileName, bool overWrite,
    //                       bool failOnWriteFailure);
  // check for valid commands
  void lookForInvalidOptions();
  void printWarnings(std::ostream &out);
  void printFlags(std::ostream &out);
  void finishSetUp(std::ostream &out);

  // option setters, case insensitive for flags
  bool setBoolOptionFalse(bool &option, std::string flag,
                          const std::string &parName, bool required = false);
  template <typename T>
  bool setOption(T &option, std::string flag, const std::string &parName,
                 bool required = false);
  // flag processing
  VecStr processFlag(std::string &flag);
  std::string getRunTime() {
  	return timer_.getStringRunTime(true, 2);
  }
  void logRunTime(std::ostream &out) {
    timer_.print(timer_.prefix_, out, 0, 2);
    return;
  }
  bool needsHelp(uint32_t minAmountOfArgs = 1);
  //void writeOutCommandLineArguments(const MapStrStr &commandLineArgumentsMap,
    //                                std::ostream &out);
};

template <typename T>
bool programSetUp::setOption(T &option, std::string flag,
                             const std::string &parName, bool required) {
  VecStr flagToks = processFlag(flag);
  validOptions_.push_back(flag);
  bool found = false;
  for (const auto &fTok : flagToks) {
    if (commands_.lookForOption(option, fTok)) {
      // validOptions_.push_back(fTok);
      pars_.addParameter(parName, option, true);
      found = true;
      break;
    } else {
      found = false;
    }
  }

  if (required && !found) {
    std::stringstream tempStream;
    tempStream << boldBlackText("Need to have ")
               << boldText(vectorToString(tokenizeString(flag, ","), " or "),
                           "31")
               << boldBlackText(" see ") +
                      boldText(commands_["-program"] + " -help ", "31") +
                      boldBlackText("for more details");
    warnings_.emplace_back(tempStream.str());
    failed_ = true;
  }
  if (!found) {
    pars_.addParameter(parName, option, false);
  }
  return found;
}
}
