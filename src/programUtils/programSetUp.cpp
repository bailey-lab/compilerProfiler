/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "programSetUp.hpp"

programSetUp::programSetUp(int argc, char *argv[]) {
  commands_ = commandLineArguments(argc, argv);
  commands_.arguments["-program"] = argv[0];
  //commands_["-program"] = argv[0];
  if(commands_.arguments.find("-program") == commands_.arguments.end()){
  	timer_ = TicToc(argv[0], false);
  }else{
  	timer_ = TicToc(commands_["-program"], false);
  }
}
programSetUp::programSetUp(const commandLineArguments &inputCommands) {
  commands_ = inputCommands;
  if(commands_.arguments.find("-program") == commands_.arguments.end()){
  	timer_ = TicToc("program", false);
  }else{
  	timer_ = TicToc(commands_["-program"], false);
  }
}
programSetUp::programSetUp(const MapStrStr &inputCommands) {
  commands_ = commandLineArguments(inputCommands);
  if(commands_.arguments.find("-program") == commands_.arguments.end()){
  	timer_ = TicToc("program", false);
  }else{
  	timer_ = TicToc(commands_["-program"], false);
  }
}

void programSetUp::lookForInvalidOptions() {
  validOptions_.push_back("-program");
  if(commands_.arguments.find("-program") != commands_.arguments.end()){
  	validOptions_.push_back(commands_["-program"]);
  }
  validOptions_.push_back("-commandline");
  changeStringVectorToLowerCase(validOptions_);
  VecStr expandedOptions;
  for (const auto &op : validOptions_) {
    addOtherVec(expandedOptions, tokenizeString(op, ","));
  }

  for (const auto &com : commands_.arguments) {
    if (!vectorContains(expandedOptions, com.first)) {
      warnings_.emplace_back(
          boldText("Unrecognized option, " + com.first + " not using", "31"));
    }
  }
}
void programSetUp::printWarnings(std::ostream &out) {
  for (const auto &warn : warnings_) {
    out << warn << std::endl;
  }
}
void programSetUp::printFlags(std::ostream &out) {
  sort(validOptions_);
  for (const auto &flag : validOptions_) {
    out << flag << std::endl;
  }
}
void programSetUp::finishSetUp(std::ostream &out) {
  if (commands_.containsFlagCaseInsensitive("-getFlags") ||
      commands_.containsFlagCaseInsensitive("-flags")) {
    printFlags(out);
    exit(1);
  }
  lookForInvalidOptions();
  printWarnings(out);
  if (failed_) {
    exit(1);
  }
}
bool programSetUp::setBoolOptionFalse(bool &option, std::string flag,
                                      const std::string &parName,
                                      bool required) {
  VecStr flagToks = processFlag(flag);
  bool found = false;
  validOptions_.push_back(flag);
  for (const auto &t : flagToks) {
    if (commands_.lookForOptionFalse(option, t)) {
      // option=true;
      // validOptions_.push_back(t);
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

VecStr programSetUp::processFlag(std::string &flag) {
  return tokenizeString(flag, ",");
}
/*
void programSetUp::writeParametersFile(const std::string &fileName,
                                       bool overWrite,
                                       bool failOnWriteFailure) {
  std::ofstream parameterFile;
  openTextFile(parameterFile, fileName, ".txt", overWrite, failOnWriteFailure);
  parameterFile << commands_.commandLine << std::endl;
  pars_.outputParsFile(parameterFile);
}*/

bool programSetUp::needsHelp(uint32_t minAmountOfArgs) {
  return (commands_.containsFlagCaseInsensitive("-help") ||
          commands_.containsFlagCaseInsensitive("-h") ||
          commands_.numberOfCommands() <= minAmountOfArgs);
}

/*
void programSetUp::writeOutCommandLineArguments(
    const MapStrStr &commandLineArgumentsMap, std::ostream &out) {
  int optionCount = 0;
  table output;
  output.hasHeader_ = true;
  output.columnNames_ = {"OptionNum", "Flag", "Option"};

  for (const auto &mContent : commandLineArgumentsMap) {
    if (mContent.first != "-commandline" && mContent.first != "-program") {
      output.content_.emplace_back(VecStr{to_string(optionCount) + ")",
                                          mContent.first, mContent.second});
      ++optionCount;
    }
  }
  output.outPutContentOrganized(out);
}*/
