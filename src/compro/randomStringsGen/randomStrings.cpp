/*
 *
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/randomStringsGen/randomStrings.hpp"
namespace compro {
std::string randStrMap(
    uint32_t size,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen) {
  std::string ans;
  ans.reserve(size);
  while (ans.size() < size) {
    double sum = 0;
    double rando = gen.unifRand();
    for (const auto &likelihood : likelihoods) {
      sum += likelihood.first;
      if (sum > rando) {
        ans.append(likelihood.second);
        break;
      }
    }
  }
  return ans;
}
std::vector<std::string> randStrsMap(
    uint32_t size,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen, uint32_t strNum) {
  std::vector<std::string> ans(strNum);
  std::generate_n(ans.begin(), strNum,
                  [&]() { return randStrMap(size, likelihoods, gen); });
  return ans;
}
std::vector<std::string> randStrsRandLenMap(
    uint32_t minLen, uint32_t maxLen,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen, uint32_t strNum) {
  std::vector<std::string> ans;
  ans.reserve(strNum);
  std::vector<uint32_t> randomLengths =
      gen.unifRandVector(minLen, maxLen + 1, strNum);
  for (const auto &len : randomLengths) {
    ans.emplace_back(randStrMap(len, likelihoods, gen));
  }
  return ans;
}

std::multimap<double, std::string, std::less<double>> getEvenLikelihood(
    const std::vector<char> &letters) {
  std::multimap<double, std::string, std::less<double>> likelihoods;
  for (const auto &let : letters) {
    likelihoods.emplace(1.0 / letters.size(), std::string(1, let));
  }
  return likelihoods;
}
std::multimap<double, std::string, std::less<double>> createLikelihood(
    const std::vector<char> &letters, const std::vector<uint32_t> &counts) {
  if (counts.size() != letters.size()) {
    std::cout << "Error in createLikelihood(const std::vector<char> &letters,"
                 " const std::vector<uint32_t> & counts)" << std::endl;
    std::cout << "Size of counts differs from size of letters" << std::endl;
    std::cout << "Size of counts: " << counts.size() << std::endl;
    std::cout << "Size of letters: " << letters.size() << std::endl;
    exit(1);
  }
  double countsSum = std::accumulate(counts.begin(), counts.end(), 0);

  std::multimap<double, std::string, std::less<double>> likelihoods;
  for (const auto &pos : iter::range(letters.size())) {
    likelihoods.emplace(counts[pos] / countsSum, std::string(1, letters[pos]));
  }
  return likelihoods;
}

std::string evenRandStr(uint32_t size, const std::vector<char> &letters,
                        const std::vector<uint32_t> &counts,
                        randomGenerator &gen) {
  return randStrMap(size, createLikelihood(letters, counts), gen);
}
std::vector<std::string> evenRandStrs(uint32_t size,
                                      const std::vector<char> &letters,
                                      const std::vector<uint32_t> &counts,
                                      randomGenerator &gen, uint32_t strNum) {
  return randStrsMap(size, createLikelihood(letters, counts), gen, strNum);
}
std::vector<std::string> evenRandStrsRandLen(
    uint32_t minLen, uint32_t maxLen, const std::vector<char> &letters,
    const std::vector<uint32_t> &counts, randomGenerator &gen,
    uint32_t strNum) {
  return randStrsRandLenMap(minLen, maxLen, createLikelihood(letters, counts),
                            gen, strNum);
}

std::string evenRandStr(uint32_t size, const std::vector<char> &letters,
                        randomGenerator &gen) {
  return randStrMap(size, getEvenLikelihood(letters), gen);
}

std::vector<std::string> evenRandStrs(uint32_t size,
                                      const std::vector<char> &letters,
                                      randomGenerator &gen, uint32_t strNum) {
  return randStrsMap(size, getEvenLikelihood(letters), gen, strNum);
}

std::vector<std::string> evenRandStrsRandLen(uint32_t minLen, uint32_t maxLen,
                                             const std::vector<char> &letters,
                                             randomGenerator &gen,
                                             uint32_t strNum) {
  return randStrsRandLenMap(minLen, maxLen, getEvenLikelihood(letters), gen,
                            strNum);
}
}
