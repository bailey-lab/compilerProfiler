#pragma once
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/common.h"
#include "compro/utils/stringUtils.hpp"
namespace compro {
template <typename NUM>
struct gapScoringParameters {
  // Constructors
  gapScoringParameters(NUM gOpen, NUM gExtend, NUM gLeftOpen, NUM gLeftExtend,
                       NUM gRightOpen, NUM gRightExtend)
      : gapOpen_(gOpen),
        gapExtend_(gExtend),
        gapRightOpen_(gRightOpen),
        gapRightExtend_(gRightExtend),
        gapLeftOpen_(gLeftOpen),
        gapLeftExtend_(gLeftExtend) {
    setIdentifer();
  };
  gapScoringParameters()
      : gapOpen_(7),
        gapExtend_(1),
        gapRightOpen_(7),
        gapRightExtend_(1),
        gapLeftOpen_(7),
        gapLeftExtend_(1) {
    setIdentifer();
  }
  gapScoringParameters(NUM gapOpen, NUM gapExtend)
      : gapOpen_(gapOpen),
        gapExtend_(gapExtend),
        gapRightOpen_(gapOpen),
        gapRightExtend_(gapExtend),
        gapLeftOpen_(gapOpen),
        gapLeftExtend_(gapExtend) {
    setIdentifer();
  }
  gapScoringParameters(const std::string& gapAll) {
    processGapStr(gapAll, gapOpen_, gapExtend_);
    processGapStr(gapAll, gapRightOpen_, gapRightExtend_);
    processGapStr(gapAll, gapLeftOpen_, gapLeftExtend_);
    setIdentifer();
  }
  gapScoringParameters(const std::string& gap, const std::string& gapLeft,
                       const std::string& gapRight) {
    processGapStr(gap, gapOpen_, gapExtend_);
    processGapStr(gapRight, gapRightOpen_, gapRightExtend_);
    processGapStr(gapLeft, gapLeftOpen_, gapLeftExtend_);
    setIdentifer();
  }

  // members
  NUM gapOpen_;
  NUM gapExtend_;
  NUM gapRightOpen_;
  NUM gapRightExtend_;
  NUM gapLeftOpen_;
  NUM gapLeftExtend_;
  std::string uniqueIdentifer_;
  // functions
  void setIdentifer() { uniqueIdentifer_ = getIdentifer(); }
  std::string getIdentifer() const {
    std::stringstream tempStream;
    tempStream << gapOpen_ << "," << gapExtend_ << "," << gapLeftOpen_ << ","
               << gapLeftExtend_ << "," << gapRightOpen_ << ","
               << gapRightExtend_;
    return tempStream.str();
  }
  void writePars(std::ostream& out) const {
    out << gapOpen_ << "," << gapExtend_ << "," << gapLeftOpen_ << ","
        << gapLeftExtend_ << "," << gapRightOpen_ << "," << gapRightExtend_
        << std::endl;
  }
  static void processGapStr(const std::string & gapStr, NUM & open, NUM & extend){
    auto gapToks = tokenizeString(gapStr, ",");
    if(gapToks.size() <2){
    	std::cout << "Gap String needs to be two numbers seperated by a comma, eg. 7,2" << std::endl
    			<< "first number is gapOpen pen and second is gapExtend pen" << std::endl;
    	exit(1);
    }
    open = std::stod(gapToks[0]);
    extend = std::stod(gapToks[1]);
  }
  bool operator==(const gapScoringParameters& otherPars) const {
    return (gapOpen_ == otherPars.gapOpen_ &&
            gapExtend_ == otherPars.gapExtend_ &&
            gapRightOpen_ == otherPars.gapRightOpen_ &&
            gapRightExtend_ == otherPars.gapRightExtend_ &&
            gapLeftOpen_ == otherPars.gapLeftOpen_ &&
            gapLeftExtend_ == otherPars.gapLeftExtend_);
  }
  bool operator>(const gapScoringParameters& otherPars) const {
    return (gapOpen_ > otherPars.gapOpen_);
  }
  bool operator<(const gapScoringParameters& otherPars) const {
    return (gapOpen_ < otherPars.gapOpen_);
  }
  virtual void printDescription(std::ostream& out, bool deep = false) const {
    out << "gapScoringParameters{" << std::endl << "gapOpen_:" << gapOpen_
        << std::endl << "gapExtend_:" << gapExtend_ << std::endl
        << "gapLeftOpen_:" << gapLeftOpen_ << std::endl
        << "gapLeftExtend_:" << gapLeftExtend_ << std::endl
        << "gapRightOpen_:" << gapRightOpen_ << std::endl
        << "gapRightExtend_:" << gapRightExtend_ << std::endl
        << "uniqueIdentifer_:" << uniqueIdentifer_ << std::endl;
    out << "}" << std::endl;
  }
};
}
