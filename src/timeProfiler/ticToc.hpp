#pragma once
/*
 *
 *

*  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "common.h"
namespace compro{
class TicToc : public zi::wall_timer {
 public:
  // Constructors
  TicToc() : prefix_(""), printAtDeath_(true), fileName_("") {}

  TicToc(const std::string s)
      : prefix_(s), printAtDeath_(true), fileName_("") {}

  TicToc(const std::string s, bool printAtDeath)
      : prefix_(s), printAtDeath_(printAtDeath), fileName_("") {}

  TicToc(const std::string s, bool printAtDeath, const std::string& fileName)
      : prefix_(s), printAtDeath_(printAtDeath), fileName_(fileName) {}

  TicToc(const std::string s, const std::string& fileName)
      : prefix_(s), printAtDeath_(true), fileName_(fileName) {}
  // deconstructor
  ~TicToc() {
    if (printAtDeath_) {
      if (fileName_ != "") {
        std::ofstream outFile;
        outFile.open(fileName_, std::ios::app);
        print(prefix_, outFile, defaultIndent_, 6);
      } else {
        print(prefix_, std::cout, defaultIndent_, 6);
      }
    }
  }
  // members
  std::string prefix_;
  bool tab_ = false;
  bool printAtDeath_;
  std::string fileName_;
  uint32_t defaultIndent_ = 0;
  // functions
  double getRunTime();
  std::string getStringRunTime(bool wordy, int decPlaces);
  void print(const std::string& pre, std::ostream& out, uint32_t indentAmount,
             int decPlaces);
};
}
