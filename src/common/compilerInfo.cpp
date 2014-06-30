/*
 * compilerInfo.cpp
 *
 *  Created on: Jun 29, 2014
 *      Author: nickhathaway
 */

#include "compilerInfo.hpp"
namespace compro{
/*
 * get compiler constants
 */
#if defined(__clang__)
std::string getCompilerVersion(){
	std::string versionStr = __VERSION__;
	if(versionStr.find("Apple") != std::string::npos){
		size_t llvmPos = versionStr.find("LLVM");
		size_t numPos = versionStr.find_first_of("0123456789", llvmPos);
		size_t notNumPos = versionStr.find_first_not_of("0123456789.", numPos);
		std::string strVerNum = versionStr.substr(numPos, notNumPos - numPos);
		return "apple_" + strVerNum;
	}else{
		size_t llvmPos = versionStr.find("Clang");
		size_t numPos = versionStr.find_first_of("0123456789", llvmPos);
		size_t notNumPos = versionStr.find_first_not_of("0123456789.", numPos);
		std::string strVerNum = versionStr.substr(numPos, notNumPos - numPos);
		return strVerNum;
	}
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


std::string getCompilerInfo(const std::string & delim, bool getHeader){
	std::stringstream tempStream;
	if(getHeader){
		tempStream << "compiler" << delim << "version"
				<< delim << "optLevel" << delim << "funrollLoops"
				<< delim << "operatingSystem";
	}else{
		tempStream << compilerUsed << delim << getCompilerVersion()
				<< delim << optimizationLevel << delim <<unrollLoopsUsed
				<< delim << operatingSystem;
	}
	return tempStream.str();
}
}
