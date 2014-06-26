#pragma once
/*
 * programRunner.hpp
 *
 *  Created on: Jun 26, 2014
 *      Author: nickhathaway
 */
#include "common.h"
#include "utils/stringUtils.hpp"

class programRunner {
 protected:
  struct funcInfo {
    std::function<int(MapStrStr)> func;
    std::string title;
    bool alias;
  };

  const std::map<std::string, funcInfo> cmdToFunc_;

 public:
  const std::string nameOfProgram_;
  programRunner(std::map<std::string, funcInfo> cmdToFunc,
                std::string nameOfProgram)
      : cmdToFunc_(cmdToFunc), nameOfProgram_(nameOfProgram) {}

  virtual ~programRunner();

  virtual int run(int argc, char* argv[]);

  virtual int runProgram(MapStrStr inputCommands) const;

  virtual void listPrograms(std::ostream& out, const std::string& command = "",
                            const std::string& nameOfProgram = "programRunner")
      const;

  virtual bool containsProgram(const std::string& program) const;
  virtual std::pair<std::string, int> closestProgram(const std::string& program)
      const;


  int runByNumber(size_t num, MapStrStr inputCommands) const;
  int runByNumber(std::string numStr, MapStrStr inputCommands) const;

 protected:
  template <typename T>
  std::pair<std::string, funcInfo> addFunc(std::string title, T& func,
                                           bool alias, bool lower = true) {
    auto name = title;
    if (lower) {
      stringToLower(name);
    }
    return {name, {std::bind(&func, std::placeholders::_1), title, alias}};
  }

  void listCommands(std::ostream& out) const {
    uint32_t offSet = 0;
    for (const auto& e : iter::enumerate(cmdToFunc_)) {
      if (e.element.second.alias) {
        ++offSet;
      } else {
        out << leftPadNumStr(e.index - offSet, cmdToFunc_.size()) << ") "
            << e.element.second.title << std::endl;
      }
    }
  }
};



