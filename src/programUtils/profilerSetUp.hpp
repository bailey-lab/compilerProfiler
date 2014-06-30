#pragma once
/*
 * profilerSetUp.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: nickhathaway
 */


#include "common.h"
#include "programSetUp.hpp"

namespace compro {

class profilerSetUp: public programSetUp {
public:
	profilerSetUp(int argc, char* argv[]) : programSetUp(argc, argv) {
   initializeDefaults();
 }
	profilerSetUp(const commandLineArguments& inputCommands)
     : programSetUp(inputCommands) {
   initializeDefaults();
 }
	profilerSetUp(const MapStrStr& inputCommands) : programSetUp(inputCommands) {
   initializeDefaults();
 }

	void initializeDefaults();

	//some defaults
	bool header_ = false;
	bool verbose_ = false;
	bool addExtra_ = false;
	std::string extraInfoStr_ = "";
	std::string extraColName_ = "";
	std::string extraColInfo_ = "";

	void processHeader();
	void processVerbose();
	void processExtra();
};

} /* namespace compro */


