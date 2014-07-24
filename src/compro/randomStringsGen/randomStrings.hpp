#pragma once
/*
 *
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/common.h"
#include "compro/randomStringsGen/randomGenerator.hpp"
namespace compro {

template<typename T, typename N>
class randObjectGen {
public:
	//constructor
	randObjectGen(const std::vector<T> & objs):
	objs_(objs), objCounts_(std::vector<N>(objs.size(),1)),
	likelihoods_(createLikelihood(objs_,objCounts_)){
		std::random_device rd;
		mtGen_.seed(rd());
	}
	randObjectGen(const std::vector<T> & objs,
			const std::vector<N> & counts):
					objs_(objs), objCounts_(counts),
	likelihoods_(createLikelihood(objs_, objCounts_)){
		std::random_device rd;
		mtGen_.seed(rd());
	}
private:
	//members
	std::mt19937_64 mtGen_;
	std::vector<T> objs_;
	std::vector<N> objCounts_;
	std::multimap<uint64_t, T, std::less<uint64_t>> likelihoods_;
public:
	//functions
	T genObj(){
	  uint64_t sum = 0;
	  uint64_t rando = mtGen_();
	  for (const auto &likelihood : likelihoods_) {
	    sum += likelihood.first;
	    if (sum > rando) {
	      return likelihood.second;
	    }
	  }
	  return likelihoods_.rbegin()->second;
	}
	std::vector<T> genObjs(uint32_t num){
		std::vector<T> ans(num);
		std::generate_n(ans.begin(), num, [&]() { return genObj();});
		return ans;
	}
	static std::multimap<uint64_t, T, std::less<uint64_t>> createLikelihood(
	    const std::vector<T> &objs, const std::vector<N> &counts){
	  if (counts.size() != objs.size()) {
	    std::cout << "Error in createLikelihood(const std::vector<T> &objs,"
	                 " const std::vector<uint32_t> & counts)" << std::endl;
	    std::cout << "Size of counts differs from size of letters" << std::endl;
	    std::cout << "Size of counts: " << counts.size() << std::endl;
	    std::cout << "Size of objs: " << objs.size() << std::endl;
	    exit(1);
	  }
	  long double countsSum = std::accumulate(counts.begin(), counts.end(), 0);
	  std::multimap<uint64_t, char, std::less<uint64_t>> likelihoods;
	  for (const auto &pos : iter::range(objs.size())) {
	    likelihoods.emplace((std::numeric_limits<uint64_t>::max() / countsSum) * counts[pos], objs[pos]);
	  }
	  return likelihoods;
	}
};

class randStrGen {
public:

	//constructor
	randStrGen(randomGenerator rGen,
			const std::vector<char> & letters):
				charGen_(randObjectGen<char,uint32_t>(letters)),
				rGen_(rGen),
				lets_(letters),
				letCounts_(std::vector<uint32_t>(letters.size(),1)){}

	randStrGen(randomGenerator rGen,
			const std::vector<char> & letters,
			const std::vector<uint32_t> & counts):
					charGen_(randObjectGen<char, uint32_t>(letters,counts)),
					rGen_(rGen),
					lets_(letters),
					letCounts_(counts){}

private:
	//members
	randObjectGen<char, uint32_t> charGen_;
	randomGenerator rGen_;
	std::vector<char> lets_;
	std::vector<uint32_t> letCounts_;
public:
	//functions
	std::string rStr(uint64_t size);
	VecStr rStrs(uint64_t size_, uint32_t num);
	std::string rStr(uint64_t minSize, uint64_t maxSize);
	VecStr rStrs(uint64_t minSize, uint64_t maxSize, uint32_t num);
};


}
