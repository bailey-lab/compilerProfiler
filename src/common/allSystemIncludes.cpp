/*
 * allSystemIncludes.cpp
 *
 *  Created on: Jun 29, 2014
 *      Author: nickhathaway
 */

#include "allSystemIncludes.h"

/*
 * get compiler constants
 */
#if defined(__clang__)
std::string getCompilerVersion(){
	std::string versionStr = __VERSION__;
	return versionStr;
}
#elif defined(__GNUC__) || defined(__GNUG__)
std::string getCompilerVersion(){
	return __VERSION__;
}
#else
std::string getCompilerVersion(){
	return __VERSION__;
}
#endif



