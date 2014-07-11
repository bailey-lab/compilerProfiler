/*
 * programRunner.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: nickhathaway
 */

#include "programRunner.hpp"

#include "alignment/aligner.hpp"
#include "programUtils/commandLineArguments.hpp"
namespace compro{
programRunner::~programRunner() {}

int programRunner::run(int argc, char *argv[]) {
  if (argc < 2) {
    listPrograms(std::cout);
    return 1;
  }
  std::stringstream commandLine;
  commandLineArguments::logRunArgumnets(commandLine, argc, argv);
  MapStrStr commands = commandLineArguments::convertCommandLineArguments(argc, argv);
  commands["-program"] = stringToLowerReturn(argv[1]);
  commands["-commandline"] = commandLine.str();
  if (stringContainsAllDigits(commands["-program"])) {
    return runByNumber(commands["-program"], commands);
  }
  return runProgram(commands);
}

int programRunner::runByNumber(size_t num, MapStrStr inputCommands) const {
  size_t offSet = 0;
  for (const auto &e : iter::enumerate(cmdToFunc_)) {
    if (e.element.second.alias) {
      ++offSet;
    } else {
      if (e.index - offSet == num) {
        const auto &fi = e.element.second;
        inputCommands["-program"] = fi.title;
        return fi.func(inputCommands);
      }
    }
  }
  std::cout << "Num: " << num
            << " is greater than size of command list: " << cmdToFunc_.size()
            << std::endl;
  return 1;
}

int programRunner::runByNumber(std::string numStr,
                               MapStrStr inputCommands) const {
  if (stringContainsAllDigits(numStr)) {
    return runByNumber(std::stoi(numStr), inputCommands);
  }
  std::cout << "NumStr isn't a number: " << numStr << std::endl;
  return 1;
}

bool programRunner::containsProgram(const std::string &program) const {
  return cmdToFunc_.find(program) != cmdToFunc_.end();
}

std::pair<std::string, int> programRunner::closestProgram(
    const std::string &program) const {
  gapScoringParameters <int32_t> gapPars(5, 1);
  alignerTemp <int32_t> alignerObj(500, gapPars, substituteMatrix(1, -1));
  std::pair<std::string, int> ans = {"", 0};
  for (const auto &prog : cmdToFunc_) {
    alignerObj.alignSeqSave(prog.first, program, false);
    if (alignerObj.parts_.score_ > ans.second) {
      ans = {prog.first, alignerObj.parts_.score_};
    }
  }
  return ans;
}

int programRunner::runProgram(MapStrStr inputCommands) const {
  if (containsProgram(inputCommands["-program"])) {
    const auto &fi = cmdToFunc_.at(inputCommands["-program"]);
    return fi.func(inputCommands);
  }
  listPrograms(std::cout, inputCommands["-program"]);
  return 1;
}

void programRunner::listPrograms(std::ostream &out, const std::string &command,
                                 const std::string &nameOfProgram) const {
  if (command != "") {
    out << "Unrecognized command " << command << std::endl;
  }
  if (nameOfProgram == nameOfProgram_) {
    out << "Programs" << std::endl;
    out << "Use " << nameOfProgram_
        << " [PROGRAM] -help to see more details about each program"
        << std::endl;
    out << "Commands are not case sensitive" << std::endl;
  } else {
    out << nameOfProgram_ << std::endl;
  }
  listCommands(out);
}
}



