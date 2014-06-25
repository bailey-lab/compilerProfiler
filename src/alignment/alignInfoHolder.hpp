#pragma once
/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "common.h"

struct gapInfo {

  gapInfo() : pos_(0), size_(0) {}
  gapInfo(uint32_t pos, uint32_t size, bool gapInA)
      : pos_(pos), size_(size), gapInA_(gapInA) {}

  //member
  uint32_t pos_;
  uint32_t size_;
  bool gapInA_;

  void writeInfo(std::ostream& out) const {
    out << pos_ << "," << size_ << "," << gapInA_ << std::endl;
  }
  void writeInfoNoEnd(std::ostream& out) const {
    out << pos_ << "," << size_ << "," << gapInA_;
  }
  bool operator>(const gapInfo& otherInfo) const {
    return pos_ > otherInfo.pos_;
  }
  bool operator<(const gapInfo& otherInfo) const {
    return pos_ < otherInfo.pos_;
  }
  bool operator==(const gapInfo& otherInfo) const {
    return (pos_ == otherInfo.pos_ && size_ == otherInfo.size_ &&
            gapInA_ == otherInfo.gapInA_);
  }
  bool operator<=(const gapInfo& otherInfo) const {
    return pos_ <= otherInfo.pos_;
  }
  bool operator>=(const gapInfo& otherInfo) const {
    return pos_ >= otherInfo.pos_;
  }
  virtual void printDescription(std::ostream& out, bool deep) const {
    out << "gapInfo{" << std::endl << "pos_:" << pos_ << std::endl
        << "size_:" << size_ << std::endl << "gapInA_:" << gapInA_ << std::endl;
    out << "}" << std::endl;
  }
};
struct alnInfoLocal {
  // Constructors
  // empty constructor
  alnInfoLocal()
      : localAStart_(0),
        localASize_(0),
        localBStart_(0),
        localBSize_(0),
        addFromFile_(false) {}
  // constructor for local alignment
  alnInfoLocal(const std::vector<gapInfo>& gInfos, uint32_t localAStart,
               uint32_t localASize, uint32_t localBStart, uint32_t localBSize,
               bool addFromFile)
      : gapInfos_(gInfos),
        localAStart_(localAStart),
        localASize_(localASize),
        localBStart_(localBStart),
        localBSize_(localBSize),
        addFromFile_(addFromFile) {}

  // members
  std::vector<gapInfo> gapInfos_;
  uint32_t localAStart_;
  uint32_t localASize_;
  uint32_t localBStart_;
  uint32_t localBSize_;
  bool addFromFile_;
  /*
  // functions
  static alnInfoLocal readInfo(std::stringstream& ss,
  		std::vector<std::string>& info, std::vector<std::string>& inputGapInfo,
  		std::vector<gapInfo> & gInfos, std::string & out);
  //
  void writeInfo(const std::string& filename, const std::string& seq1,
                 const std::string& seq2, std::ostream& indexFile) const;
  void writeInfoSingleLine(std::ostream& indexFile, uint64_t seq1,
                           uint64_t seq2) const;*/

  virtual void printDescription(std::ostream& out, bool deep) const {
    out << "localAStart_" << localAStart_ << std::endl << "localASize_"
        << localASize_ << std::endl << "localBStart_" << localBStart_
        << std::endl << "localBSize_" << localBSize_ << std::endl;
    if (deep) {
      out << "gapInfo:" << std::endl;
      out << "std::vector<gapInfo> gapInfos_" << std::endl;
      for (const auto& g : gapInfos_) {
        g.printDescription(out, deep);
      }
    }
    out << "}" << std::endl;
  }

};

struct alnInfoGlobal {
  // contructors
  // empty construcor
  alnInfoGlobal() : addFromFile_(false) {}
  // contructor for global alingment
  alnInfoGlobal(const std::vector<gapInfo>& gInfos, bool addFromFile)
      : gapInfos_(gInfos), addFromFile_(addFromFile) {}

  // members
  std::vector<gapInfo> gapInfos_;
  bool addFromFile_;
  /*
  //function
  static alnInfoGlobal readInfo(std::stringstream& ss,
    		std::vector<std::string>& info, std::vector<std::string>& inputGapInfo,
    		std::vector<gapInfo> & gInfos, std::string & out);

  void writeInfo(const std::string& filename, const std::string& seq1,
                 const std::string& seq2, std::ostream& indexFile) const;

  void writeInfoSingleLine(std::ostream& indexFile, uint64_t seq1,
                           uint64_t seq2) const;*/
  virtual void printDescription(std::ostream& out, bool deep) const {
    if (deep) {
      out << "gapInfo:" << std::endl;
      out << "std::vector<gapInfo> gapInfos_" << std::endl;
      for (const auto& g : gapInfos_) {
        g.printDescription(out, deep);
      }
    }
    out << "}" << std::endl;
  }
};
