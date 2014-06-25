#pragma once
/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "common.h"
#include "randomStringsGen/randomGenerator.hpp"
std::string randStrMap(
    uint32_t size,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen) ;
std::vector<std::string> randStrsMap(
    uint32_t size,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen, uint32_t strNum);
std::vector<std::string> randStrsRandLenMap(
    uint32_t minLen, uint32_t maxLen,
    const std::multimap<double, std::string, std::less<double>> &likelihoods,
    randomGenerator &gen, uint32_t strNum);

std::multimap<double, std::string, std::less<double>> getEvenLikelihood(
    const std::vector<char> &letters);
std::multimap<double, std::string, std::less<double>> createLikelihood(
    const std::vector<char> &letters, const std::vector<uint32_t> & counts);

std::string evenRandStr(uint32_t size, const std::vector<char> &letters,
		const std::vector<uint32_t> & counts,
                        randomGenerator &gen);
std::vector<std::string> evenRandStrs(uint32_t size, const std::vector<char> &letters,
		const std::vector<uint32_t> & counts,
                    randomGenerator &gen, uint32_t strNum);
std::vector<std::string> evenRandStrsRandLen(uint32_t minLen, uint32_t maxLen,
                           const std::vector<char> &letters, const std::vector<uint32_t> & counts,
                           randomGenerator &gen, uint32_t strNum);

std::string evenRandStr(uint32_t size, const std::vector<char> &letters,
                        randomGenerator &gen);

std::vector<std::string> evenRandStrs(uint32_t size, const std::vector<char> &letters,
                    randomGenerator &gen, uint32_t strNum);

std::vector<std::string> evenRandStrsRandLen(uint32_t minLen, uint32_t maxLen,
                           const std::vector<char> &letters,
                           randomGenerator &gen, uint32_t strNum);
