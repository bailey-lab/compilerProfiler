#pragma once
/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include <stdio.h>
#include <cstddef>
#include <utility>
#include <iterator>
#include <iostream>
#include <vector>
#include <array>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include <errno.h>
#include <exception>
#include <stdexcept>
#include <memory>
#include <thread>
// For toupper()
#include <cctype>

// For isdigit()
#include <ctype.h>

// Below is for file modification times
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <ctime>
#include <sys/types.h>
#include <regex.h>
#include <functional>

#include <cppitertools/enumerate.hpp>
#include <cppitertools/range.hpp>
#include <zi/time/time.hpp>

/*
 * get compiler constants
 */
std::string getCompilerVersion();
#if defined(__clang__)
static std::string compilerVersion = "clang";
#elif defined(__GNUC__) || defined(__GNUG__)
static std::string compilerVersion =  "gxx";
#else
static std::string compilerVersion = "unrecognized";
#endif

/*
 * get operating system
 */
#if defined(__linux__)
static std::string operatingSystem = "linux";
#elif defined(__APPLE__)
static std::string operatingSystem = "mac";
#else
static std::string operatingSystem = "other";
#endif

/*
 * define optimization
 */
#if defined(OPTLEV1)
static std::string optimizationLevel = "O1";
#elif defined(OPTLEV2)
static std::string optimizationLevel = "O2";
#elif defined(OPTLEV3)
static std::string optimizationLevel = "O3";
#else
static std::string optimizationLevel = "none";
#endif
