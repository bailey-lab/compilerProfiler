#pragma once
/*
 * 
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"
namespace compro{
class commandLineArguments {

 public:
  commandLineArguments() {}
  commandLineArguments(int argc, char* argv[])
      : arguments(convertCommandLineArguments(argc, argv)) {
    std::stringstream hold;
    logRunArgumnets(hold, argc, argv);
    commandLine = hold.str();
  }
  commandLineArguments(const MapStrStr& inputCommands)
      : arguments(inputCommands) {
    lookForOption(commandLine, "-commandline");
  }
  //members

  MapStrStr arguments;
  std::string commandLine;

 private:
  std::string& lowerCaseGetArguments(const std::string& str) ;

 public:
  std::string& operator[](const std::string& str);
  // string
  bool lookForOption(std::string& option, const std::string& flag);
  // bool
  bool lookForOption(bool& option, const std::string& flag) ;
  // bool false
  bool lookForOptionFalse(bool& option, const std::string& flag) ;
  // int
  bool lookForOption(int& option, const std::string& flag) ;
  // int_64t
  bool lookForOption(int64_t& option, const std::string& flag);
  // size_t
  //bool lookForOption(size_t& option, const std::string& flag) ;
  // uint32_t
  bool lookForOption(uint32_t& option, const std::string& flag) ;
  // uint64_t
  bool lookForOption(uint64_t& option, const std::string& flag) ;

  // double
  bool lookForOption(double& option, const std::string& flag);
  bool containsFlagCaseSensitive(const std::string& flag);
  bool containsFlagCaseInsensitive(const std::string& flag);

  static void logRunArgumnets(std::ostream& out, int argc, char* argv[]);
  static void logRunArgumnets(std::ostream& out, const std::string& commandLine);
  static MapStrStr convertCommandLineArguments(int argc, char* argv[]);
  uint32_t numberOfCommands() ;
};
}
