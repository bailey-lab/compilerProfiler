#pragma once
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"
#include "compro/utils.h"
namespace compro {
class parameter {
 public:
  // bool constructor
  parameter(const std::string& parName, bool inValue, bool commandLine) {
    name = parName;
    value = convertBoolToString(inValue);
    if (commandLine) {
      defaultOrCommandline = "commandLine";
    } else {
      defaultOrCommandline = "default";
    }
  }
  // string contstructor
  parameter(const std::string& parName, const std::string& inValue,
            bool commandLine) {
    if (inValue == "") {
      value = "none";
    } else {
      value = inValue;
    }
    name = parName;
    if (commandLine) {
      defaultOrCommandline = "commandLine";
    } else {
      defaultOrCommandline = "default";
    }
  }

  template <typename T>
  parameter(const std::string& parName, T inValue, bool commandLine) {
    name = parName;
    value = to_string(inValue);
    if (commandLine) {
      defaultOrCommandline = "commandLine";
    } else {
      defaultOrCommandline = "default";
    }
  }

  std::string name;
  std::string value;
  std::string defaultOrCommandline;
  std::string outputInfoString() {
    return name + "\t" + value + "\t" + defaultOrCommandline;
  }
};

class parametersHolder {

 public:
  parametersHolder() {}
  parametersHolder(const parameter& firstParameter)
      : pars(std::vector<parameter>(1, firstParameter)) {}

  std::vector<parameter> pars;
  void addParameter(const parameter& newParameter) {
    pars.push_back(newParameter);
  }
  template <typename T>
  void addParameter(const std::string& parName, const T& inValue,
                    bool commandLine) {
    pars.push_back(parameter(parName, inValue, commandLine));
  }
  void outputParsFile(std::ostream& out) {
    out << "ParameterName\tvalue\tcommandLineOrDefault" << std::endl;
    for (auto& pIter : pars) {
      out << pIter.outputInfoString() << std::endl;
    }
  }
};
}
