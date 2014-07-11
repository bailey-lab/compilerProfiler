/*
 * profilerSetUp.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: nickhathaway
 */

#include "compro/programUtils/profilerSetUp.hpp"

namespace compro {

void profilerSetUp::initializeDefaults(){
	processHeader();
	processVerbose();
	processExtra();
	processLogFileName();
	//only add the header if file doesn't exist if logging to file
	if(logFileName_ != ""){
		header_ = !(fexists(logFileName_));
	}
}

void profilerSetUp::processHeader(){
	setOption(header_, "-header", "header");
}

void profilerSetUp::processVerbose(){
	setOption(verbose_, "-verbose,-v", "verbose");
}
void profilerSetUp::processLogFileName(){
	setOption(logFileName_, "-log,-logFilename", "logFilename");
}

void profilerSetUp::processExtra(){
	if(setOption(extraInfoStr_, "-extra,-extraCol,-extraInfo", "extra")){
		auto toks = tokenizeString(extraInfoStr_, ",");
		for(const auto & tok : toks){
			auto subToks = tokenizeString(tok, ":");
			if(subToks.size() != 2){
				std::cout << "Error, extra info needs to two strings separated by a colon, eg. COLNAME:COLINFO" << std::endl;
				std::cout << "This was given " << extraInfoStr_ <<std::endl;
			}else{
				extraInfo_.emplace_back(std::make_pair(subToks[0], subToks[1]));
			}
		}
	}
}

} /* namespace compro */
