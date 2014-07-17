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


/*
std::string randStrMap(
    uint32_t size,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen);

VecStr randStrsMap(
    uint32_t size,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen, uint32_t strNum);
VecStr randStrsRandLenMap(
    uint32_t minLen, uint32_t maxLen,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen, uint32_t strNum);

std::multimap<double, std::string, std::less<double>> getEvenLikelihood(
    const std::vector<char> &letters);
std::multimap<double, std::string, std::less<double>> createLikelihood(
    const std::vector<char> &letters, const std::vector<uint32_t> &counts);

std::string evenRandStr(uint32_t size, const std::vector<char> &letters,
                        const std::vector<uint32_t> &counts,
                        randomGenerator &gen);
VecStr evenRandStrs(uint32_t size,
                                      const std::vector<char> &letters,
                                      const std::vector<uint32_t> &counts,
                                      randomGenerator &gen, uint32_t strNum);
VecStr evenRandStrsRandLen(
    uint32_t minLen, uint32_t maxLen, const std::vector<char> &letters,
    const std::vector<uint32_t> &counts, randomGenerator &gen, uint32_t strNum);

std::string evenRandStr(uint32_t size, const std::vector<char> &letters,
                        randomGenerator &gen);

VecStr evenRandStrs(uint32_t size,
                                      const std::vector<char> &letters,
                                      randomGenerator &gen, uint32_t strNum);

VecStr evenRandStrsRandLen(uint32_t minLen, uint32_t maxLen,
                                             const std::vector<char> &letters,
                                             randomGenerator &gen,
                                             uint32_t strNum);
*/

class randStrGen {
public:

	//constructor
	randStrGen(randomGenerator rGen,
			const std::vector<char> & letters): rGen_(rGen),
	lets_(letters), letCounts_(std::vector<uint32_t>(letters.size(),1)),
	letLikihoods_(createLikelihood(lets_,letCounts_)){}

	randStrGen(randomGenerator rGen,
			const std::vector<char> & letters,
			const std::vector<uint32_t> & counts):rGen_(rGen),
	lets_(letters), letCounts_(counts),
	letLikihoods_(createLikelihood(lets_,letCounts_)){}

private:
	//members
	randomGenerator rGen_;
	std::vector<char> lets_;
	std::vector<uint32_t> letCounts_;
	std::multimap<double, char, std::less<double>> letLikihoods_;
public:
	//functions
	// Get a random string of size of maxLen_
	char genRChar();
	std::string rStr(uint64_t size);
	VecStr rStrs(uint64_t size_, uint32_t num);
	std::string rStr(uint64_t minSize, uint64_t maxSize);
	VecStr rStrs(uint64_t minSize, uint64_t maxSize, uint32_t num);

	static std::multimap<double, char, std::less<double>> createLikelihood(
	    const std::vector<char> &letters, const std::vector<uint32_t> &counts);
};
class randStrGenTest {
public:

	//constructor
	randStrGenTest(randomGenerator rGen,
			const std::vector<char> & letters): rGen_(rGen),
	lets_(letters), letCounts_(std::vector<uint32_t>(letters.size(),1)),
	letLikihoods_(createLikelihood(lets_,letCounts_)){}

	randStrGenTest(randomGenerator rGen,
			const std::vector<char> & letters,
			const std::vector<uint32_t> & counts):rGen_(rGen),
	lets_(letters), letCounts_(counts),
	letLikihoods_(createLikelihood(lets_,letCounts_)){}

private:
	//members
	randomGenerator rGen_;
	std::vector<char> lets_;
	std::vector<uint32_t> letCounts_;
	std::multimap<uint64_t, char, std::less<uint64_t>> letLikihoods_;
public:
	//functions
	// Get a random string of size of maxLen_
	char genRChar();
	std::string rStr(uint64_t size);
	VecStr rStrs(uint64_t size_, uint32_t num);
	std::string rStr(uint64_t minSize, uint64_t maxSize);
	VecStr rStrs(uint64_t minSize, uint64_t maxSize, uint32_t num);

	static std::multimap<uint64_t, char, std::less<uint64_t>> createLikelihood(
	    const std::vector<char> &letters, const std::vector<uint32_t> &counts);
};

template<typename T>
class randObjectGen {
public:

	//constructor
	randObjectGen(const std::vector<T> & objs):
	objs_(objs), objCounts_(std::vector<uint32_t>(objs.size(),1)),
	likelihoods_(createLikelihood(objs_,objCounts_)),
	likelihoodsDoub_(createLikelihoodDoub(objs_,objCounts_)){
		std::random_device rd;
		mtGen_.seed(rd());
	}

	randObjectGen(const std::vector<T> & letters,
			const std::vector<uint32_t> & counts):
					objs_(letters), objCounts_(counts),
	likelihoods_(createLikelihood(objs_,objCounts_)),
	likelihoodsDoub_(createLikelihoodDoub(objs_,objCounts_)){
		std::random_device rd;
		mtGen_.seed(rd());
	}

private:
	//members
	std::mt19937_64 mtGen_;
	std::vector<T> objs_;
	std::vector<uint32_t> objCounts_;
	std::multimap<uint64_t, T, std::less<uint64_t>> likelihoods_;
	std::multimap<double, T, std::less<double>> likelihoodsDoub_;
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
	T genObjDoub(){
	  double sum = 0;
	  double rando = mtGen_()/static_cast<double>(mtGen_.max());
	  for (const auto &likelihood : likelihoodsDoub_) {
	    sum += likelihood.first;
	    if (sum > rando) {
	      return likelihood.second;
	    }
	  }
	  return likelihoodsDoub_.rbegin()->second;
	}
	std::vector<T> genObjs(uint32_t num){
		std::vector<T> ans;
		std::generate_n(ans.begin(), num,  [&]() { return genObj();});


		return ans;
	}
	static std::multimap<uint64_t, T, std::less<uint64_t>> createLikelihood(
	    const std::vector<T> &objs, const std::vector<uint32_t> &counts){
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
	static std::multimap<double, T, std::less<double>> createLikelihoodDoub(
		    const std::vector<T> &objs, const std::vector<uint32_t> &counts){
		if (counts.size() != objs.size()) {
			std::cout << "Error in createLikelihood(const std::vector<T> &objs,"
									 " const std::vector<uint32_t> & counts)" << std::endl;
			std::cout << "Size of counts differs from size of letters" << std::endl;
			std::cout << "Size of counts: " << counts.size() << std::endl;
			std::cout << "Size of objs: " << objs.size() << std::endl;
			exit(1);
		}
		double countsSum = std::accumulate(counts.begin(), counts.end(), 0);

		std::multimap<double, char, std::less<double>> likelihoods;
		for (const auto &pos : iter::range(objs.size())) {
			likelihoods.emplace((counts[pos] / countsSum) , objs[pos]);
		}
		return likelihoods;
	}
};
}
