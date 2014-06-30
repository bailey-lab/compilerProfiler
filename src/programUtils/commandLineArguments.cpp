/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "commandLineArguments.hpp"
#include "utils/stringUtils.hpp"
namespace compro{
std::string& commandLineArguments::lowerCaseGetArguments(const std::string& str) {
  if (arguments[stringToLowerReturn(str)] == "") {
    std::cout << "Argument " << str
              << " requires an option but none was given" << std::endl;
    exit(1);
  }
  return arguments[stringToLowerReturn(str)];
}

std::string& commandLineArguments::operator[](const std::string& str) {
  return lowerCaseGetArguments(str);
}

// string
bool commandLineArguments::lookForOption(std::string& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = lowerCaseGetArguments(flag);
    return true;
  } else {
    return false;
  }
}
// bool
bool commandLineArguments::lookForOption(bool& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = true;
    return true;
  } else {
    return false;
  }
}
// bool false
bool commandLineArguments::lookForOptionFalse(bool& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = false;
    return true;
  } else {
    return false;
  }
}
// int
bool commandLineArguments::lookForOption(int& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
// int_64t
bool commandLineArguments::lookForOption(int64_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
/*
// size_t
bool commandLineArguments::lookForOption(size_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}*/

// uint32_t
bool commandLineArguments::lookForOption(uint32_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoi(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}

// uint64_t
bool commandLineArguments::lookForOption(uint64_t& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stoull(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}

// double
bool commandLineArguments::lookForOption(double& option, const std::string& flag) {
  if (containsFlagCaseInsensitive(flag)) {
    option = std::stof(lowerCaseGetArguments(flag));
    return true;
  } else {
    return false;
  }
}
bool commandLineArguments::containsFlagCaseSensitive(const std::string& flag) {
  if (arguments.find(flag) != arguments.end()) {
    return true;
  } else {
    return false;
  }
}
bool commandLineArguments::containsFlagCaseInsensitive(const std::string& flag) {
  if (arguments.find(stringToLowerReturn(flag)) != arguments.end()) {
    return true;
  } else {
    return false;
  }
}


uint32_t commandLineArguments::numberOfCommands() {
  int offSet = 0;
  if (arguments.find("-program") != arguments.end()) {
    ++offSet;
  }
  if (arguments.find("-commandline") != arguments.end()) {
    ++offSet;
  }
  return arguments.size() - offSet;
}

void commandLineArguments::logRunArgumnets(std::ostream& out, int argc, char* argv[]){
  std::string directory = get_cwd();

  out << "Command ran from " << directory << std::endl;
  for (int args = 0; args < argc; ++args) {
    out << argv[args] << " ";
  }
  out << std::endl << std::endl;
}
void commandLineArguments::logRunArgumnets(std::ostream& out, const std::string& commandLine){
	out << commandLine;
}

MapStrStr commandLineArguments::convertCommandLineArguments(int argc, char* argv[]){
  MapStrStr storage;
  for (int i = 1; i < argc; i++) {
    std::string nextParam = stringToLowerReturn(argv[i]);
    if (storage.find(nextParam) != storage.end()) {
      std::cout << "Error, already have " << nextParam << std::endl;
      std::cout << "Check if you entered it in twice" << std::endl;
      exit(1);
    }
    if (argv[i][0] == '-') {
      if (i + 1 >= argc) {
        storage.insert(std::make_pair(stringToLowerReturn(argv[i]), ""));
      } else {
        if (argv[i + 1][0] == '-') {
          storage.insert(std::make_pair(stringToLowerReturn(argv[i]), ""));
        } else {
          storage.insert(
              std::make_pair(stringToLowerReturn(argv[i]), argv[i + 1]));
          i++;
        }
      }
    } else {
      storage.insert(std::make_pair(stringToLowerReturn(argv[i]), ""));
    }
  }
  return storage;
}
}
