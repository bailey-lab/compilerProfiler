#pragma once
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"
#include "compro/utils/vectorUtils.hpp"
namespace compro {
class randomGenerator {

 public:
  randomGenerator() {
    seed();
    needToSeed_ = false;
  }
  /*
  randomGenerator(uint64_t givenSeed) {
        seedNum(givenSeed);
    needToSeed_ = false;
  }*/
  // Members
  std::mt19937_64 mtGen_;
  bool needToSeed_;
  // Generate a random number between 0 and 1 not inclusive
  double unifRand() {
  	return mtGen_() /( static_cast<double>(mtGen_.max()) + 1);
  }
  double operator()() { return unifRand(); }
  // return a vector of random numbers between 0 and 1
  std::vector<double> unifRandVector(uint32_t num) {
    std::vector<double> ans(num);
    std::generate(ans.begin(), ans.end(),[&](){
    	return unifRand();} );
    return ans;
  }
  // return random number between start and stop, not inclusive of stop if T is
  // int type...i think
  template <typename T>
  T unifRand(T start, T stop) {
    return (T)(stop - start) * unifRand() + start;
  }
  template <typename T>
  T unifRandSelection(const std::vector<T>& vec) {
    return vec[unifRand<uint32_t>(0, vec.size())];
  }
  template <typename T>
  std::vector<T> unifRandSelectionVec(const std::vector<T>& vec, uint32_t amt,
                                      bool withReplacement) {
    std::vector<T> ans;
    std::vector<uint32_t> rSel;
    if (withReplacement) {
      rSel = unifRandVector<uint32_t>(0, vec.size(), amt);
    } else {
      if (amt > vec.size()) {
        std::cout << "Error in unifRandSelectionVec, requesting more than is "
                     "in vec but said without replacement" << std::endl;
        // std::cout << "Just return the original vector" << std::endl;
        exit(1);
      } else {
        std::vector<uint32_t> rSelPos(vec.size());
        std::iota(rSel.begin(), rSel.end(), 0);
        std::shuffle(rSel.begin(), rSel.end(), mtGen_);
        rSel = getSubVector<uint32_t>(rSelPos, 0, amt);
      }
    }
    for (const auto& pos : rSel) {
      ans.emplace_back(vec[pos]);
    }
    return ans;
  }
  template <typename T>
  std::vector<T> unifRandVector(T start, T stop, int num) {
    std::vector<T> ans(num);
    std::generate_n(ans.begin(), num, [&]() { return unifRand(start, stop); });
    return ans;
  }
  template <typename T>
  std::vector<std::vector<T>> unifRandVecVec(T start, T stop, uint32_t totalNum,
                                             uint32_t subNum) {
    std::vector<std::vector<T>> ans(totalNum);
    std::generate(ans.begin(), ans.end(),
                  [&]() { return unifRandVector<T>(start, stop, subNum); });
    return ans;
  }
  // Reset the random number generator with the system clock.
  void seed() {
    std::random_device rd;
    mtGen_.seed(rd());
  }
  void seedNum(uint64_t givenSeed) { mtGen_.seed(givenSeed); }
};
}
