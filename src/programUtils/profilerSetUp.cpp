/*
 * profilerSetUp.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: nickhathaway
 */

#include "profilerSetUp.hpp"

namespace compro {

void profilerSetUp::initializeDefaults(){
	processHeader();
	processVerbose();
}

void profilerSetUp::processHeader(){
	setOption(header_, "-header", "header");
}

void profilerSetUp::processVerbose(){
	setOption(verbose_, "-verbose,-v", "verbose");
}

void profilerSetUp::processExtra(){
	addExtra_ = setOption(extraInfoStr_, "-extra,-extraCol,-extraInfo", "extra");
	if(addExtra_){
		auto toks = tokenizeString(extraInfoStr_, ":");
		if(toks.size() != 2){
			std::cout << "Error, extra info needs to two strings seperated by a colon, eg. COLNAME:COLINFO" << std::endl;
			std::cout << "This was given " << extraInfoStr_ <<std::endl;
		}else{
			extraColName_ = toks[0];
			extraColInfo_ = toks[1];
		}
	}
}

} /* namespace compro */
