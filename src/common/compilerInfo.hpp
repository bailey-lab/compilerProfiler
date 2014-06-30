#pragma once
/*
 * compilerInfo.hpp
 *
 *  Created on: Jun 29, 2014
 *      Author: nickhathaway
 */

#include "allSystemIncludes.h"
namespace compro{
/*
 * get compiler version
 */
std::string getCompilerVersion();

/*
 * get compiler
 */

#if defined(__clang__)
static std::string compilerUsed = "clang";
#elif defined(__GNUC__) || defined(__GNUG__)
static std::string compilerUsed =  "gxx";
#else
static std::string compilerUsed = "unrecognized";
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

/*
 * find if funroll-loops is being used
 */
#if defined(UNROLL_LOOPS)
static std::string unrollLoopsUsed = "true";
#else
static std::string unrollLoopsUsed = "false";
#endif

/*
 * doens't seem to be a way to get processor difference in
 */


std::string getCompilerInfo(const std::string & delim, bool getHeader);

}
