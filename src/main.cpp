
/*

 * main.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway
 */



#include <stdio.h>
#include <iostream>
#include <vector>
#include <array>
#include <stdint.h>
#include "profilerRunner.hpp"

using namespace compro;

int simpleAlignmentProfiler(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 40;
	uint32_t strNum = 50;
	uint32_t runTimes = 1;
	bool local = false;
	std::string alphStr = "A40,C10,G10,T40";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	bool simple = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	setUp.setOption(minSize, "-minSize", "minSize");
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(simple, "-simple", "simple");
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(runTimes, "-runTimes", "runTimes");
	setUp.setOption(local, "-local", "local");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");

	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "runTimes: " << runTimes << std::endl;
		std::cout << "local: " << local << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	gapScoringParameters <int32_t> gapPars(5,1);
	substituteMatrix scoreMatrix(2,-2);
	aligner <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
	for(uint32_t run = 0; run < runTimes; ++run){
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		alignerObj.alignSeq(randStrings.front(), randStrings.back(), local);
		if(veryVerbose){
			std::for_each(randStrings.begin(),
						randStrings.end(), [&](const std::string & str){ std::cout << str << std::endl;});
		}
		if (setUp.verbose_) {
			std::cout << alignerObj.alignObjectA_ << std::endl;
			std::cout << alignerObj.alignObjectB_ << std::endl;
		}
		for(auto pos : iter::range(randStrings.size())){
			for(auto secondPos : iter::range(randStrings.size())){
				alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], local);
			}
		}
	}
	if(setUp.verbose_){
		setUp.logRunTime(std::cout);
	}else{
		std::cout << setUp.timer_.getRunTime() << std::endl;
	}
	return 0;
}



int fullAlignmentProfiler(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 50;
	uint32_t strNum = 50;
	uint32_t runTimes = 1;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";

	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	if(!setUp.setOption(minSize, "-minSize", "minSize")){
		minSize = maxSize;
	}
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(runTimes, "-runTimes", "runTimes");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "runTimes: " << runTimes << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	setUp.extraInfo_.emplace_back("minSize", to_string(minSize));
	setUp.extraInfo_.emplace_back("maxSize", to_string(maxSize));
	setUp.extraInfo_.emplace_back("strNum", to_string(strNum));
	setUp.extraInfo_.emplace_back("runTimes", to_string(runTimes));

	if(setUp.header_){
		setUp.logging_ << "numType\talnType\talnCount\t"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
							++alnCount;
						}
					}
				}
			setUp.logging_ <<"double\tglobal\t" << alnCount << "\t"
										<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
										<< std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"float\tglobal\t" << alnCount << "\t"
										<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
										<< std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int16_t\tglobal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int32_t\tglobal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int64_t\tglobal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
							++alnCount;
						}
					}
				}
			setUp.logging_ <<"double\tlocal\t" << alnCount << "\t"
										<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
										<< std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"float\tlocal\t" << alnCount << "\t"
										<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
										<< std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int16_t\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int32_t\tlocal\t"  << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeq(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int64_t\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}

	if(setUp.verbose_){
		setUp.logRunTime(std::cout);
	}
	return 0;
}


int justScoreAlignmentProfiler(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 50;
	uint32_t strNum = 50;
	uint32_t runTimes = 1;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	if(!setUp.setOption(minSize, "-minSize", "minSize")){
		minSize = maxSize;
	}
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(runTimes, "-runTimes", "runTimes");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "runTimes: " << runTimes << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	setUp.extraInfo_.emplace_back("minSize", to_string(minSize));
	setUp.extraInfo_.emplace_back("maxSize", to_string(maxSize));
	setUp.extraInfo_.emplace_back("strNum", to_string(strNum));
	setUp.extraInfo_.emplace_back("runTimes", to_string(runTimes));
	if(setUp.header_){
		setUp.logging_ <<"numType\talnType\talnCount"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
							++alnCount;
						}
					}
				}
			setUp.logging_ <<"double\tglobal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
					<< std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"float\tglobal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
					<< std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int16_t\tglobal\t"<< alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int32_t\tglobal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], false);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int64_t\tglobal\t"<< alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * double
		 */
		gapScoringParameters <double> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <double> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("double", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){

					for(auto pos : iter::range(randStrings.size())){
						for(auto secondPos : iter::range(randStrings.size())){
							alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
							++alnCount;
						}
					}
				}
			setUp.logging_ <<"double\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
					<< std::endl;
			}
		}
	{
		/*
		 * float
		 */
		gapScoringParameters <float> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <float> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerdoub("float", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"float\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerdoub)
					<< std::endl;
		}
	}
	{
		/*
		 * int16_t
		 */
		gapScoringParameters <int16_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int16_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int16_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int16_t\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int32_t
		 */
		gapScoringParameters <int32_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int32_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
					alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int32_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int32_t\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}
	{
		/*
		 * int64_t
		 */
		gapScoringParameters <int64_t> gapPars(5,1);
		substituteMatrix scoreMatrix(2,-2);
		aligner <int64_t> alignerObj(maxSize, gapPars, scoreMatrix);
		std::vector<std::string> randStrings = evenRandStrsRandLen(minSize, maxSize,
				alphabet, alphCounts,  gen, strNum);
		{
			timeTracker timmerInt("int64_t", false);
			uint64_t alnCount = 0;
			for(uint32_t run = 0; run < runTimes; ++run){
				for(auto pos : iter::range(randStrings.size())){
					for(auto secondPos : iter::range(randStrings.size())){
						alignerObj.alignSeqSave(randStrings[pos], randStrings[secondPos], true);
						++alnCount;
					}
				}
			}
			setUp.logging_ <<"int64_t\tlocal\t" << alnCount << "\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmerInt)
					<< std::endl;
		}
	}

	if(setUp.verbose_){
		setUp.logRunTime(std::cout);
	}
	return 0;
}

int randomNumberGeneration(MapStrStr inputCommands) {
	profilerSetUp setUp(inputCommands);
	uint64_t stop = 100;
	bool randDeviceAsWell = false;
	setUp.setOption(randDeviceAsWell, "-randDev", "randDeviceAsWell");
	setUp.setOption(stop, "-stop", "stop");
	setUp.finishSetUp(std::cout);
	std::random_device rd;
	std::mt19937 mtGen(rd());
	std::mt19937_64 mtGen64(rd());
	std::uniform_int_distribution<> disIntMt(1, 6);
	std::uniform_int_distribution<> disIntMt64(1, 6);
	std::uniform_real_distribution<> disDoubMt(0, 1);
	std::uniform_real_distribution<> disDoubMt64(0, 1);
	randomGenerator gen;
	//std::cout << mtGen.max() << std::endl;
	//std::cout << mtGen64.max() << std::endl;
	setUp.extraInfo_.emplace_back("runTimes", to_string(stop));
	if(setUp.header_){
		setUp.logging_ <<"generator\t"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}
	if(randDeviceAsWell){
		timeTracker timmer("random_device", false);
		for(uint64_t run = 0; run < stop; ++run	){
			rd();
		}
		setUp.logging_ <<"random_device\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("mt",false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen();
		}
		setUp.logging_ <<"mt19937\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("mt64", false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen64();
		}
		setUp.logging_ <<"mt19937_64\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("randomGenerator", false);
		for(uint64_t run = 0; run < stop; ++run	){
			gen.unifRand();
		}
		setUp.logging_ <<"randomGenerator\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("uniIntMt",false);
		for(uint64_t run = 0; run < stop; ++run	){
			disIntMt(mtGen);
		}
		setUp.logging_ <<"uniIntMt\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("uniIntMt64", false);
		for(uint64_t run = 0; run < stop; ++run	){
			disIntMt64(mtGen64);
		}
		setUp.logging_ <<"uniIntMt64\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("uniDoubMt",false);
		for(uint64_t run = 0; run < stop; ++run	){
			disDoubMt(mtGen);
		}
		setUp.logging_ <<"uniDoubMt\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("uniDoubMt64", false);
		for(uint64_t run = 0; run < stop; ++run	){
			disDoubMt64(mtGen64);
		}
		setUp.logging_ <<"uniDoubMt64\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	return 0;
}

int customRandomGenerator(MapStrStr inputCommands) {
	profilerSetUp setUp(inputCommands);
	uint64_t stop = 100;
	bool randDeviceAsWell = false;
	setUp.setOption(randDeviceAsWell, "-randDev", "randDeviceAsWell");
	setUp.setOption(stop, "-stop", "stop");
	setUp.finishSetUp(std::cout);
	std::random_device rd;
	std::mt19937 mtGen(rd());
	std::mt19937_64 mtGen64(rd());
	randomGenerator gen;
	//std::cout << mtGen.max() << std::endl;
	//std::cout << mtGen64.max() << std::endl;
	if(setUp.header_){
		setUp.logging_ <<"generator\t"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}
	if(randDeviceAsWell){
		timeTracker timmer("random_device", false);
		for(uint64_t run = 0; run < stop; ++run	){
			rd();
		}
		setUp.logging_ <<"random_device\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("mt",false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen();
		}
		setUp.logging_ <<"mt19937\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("mt64", false);
		for(uint64_t run = 0; run < stop; ++run	){
			mtGen64();
		}
		setUp.logging_ <<"mt19937_64\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	{
		timeTracker timmer("randomGenerator", false);
		for(uint64_t run = 0; run < stop; ++run	){
			gen.unifRand();
		}
		setUp.logging_ <<"randomGenerator\t"
							<< getRunInfo("\t", false, setUp.extraInfo_, timmer) << "\t"
							<< timmer.getRunTime() << std::endl;
	}
	return 0;
}

int mapVsUnorderedMap(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 50;
	uint32_t strNum = 50;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	if(!setUp.setOption(minSize, "-minSize", "minSize")){
		minSize = maxSize;
	}
	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	setUp.extraInfo_.emplace_back("minSize", to_string(minSize));
	setUp.extraInfo_.emplace_back("maxSize", to_string(maxSize));
	setUp.extraInfo_.emplace_back("strNum", to_string(strNum));
	if(setUp.header_){
		setUp.logging_ <<"mapType\t"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}

	VecStr randoms = evenRandStrsRandLen(minSize,maxSize,
			alphabet, alphCounts,  gen, strNum);
	VecStr randomsMix = evenRandStrsRandLen(minSize,maxSize,
			alphabet, alphCounts,  gen, strNum/2);
	addOtherVec(randomsMix, gen.unifRandSelectionVec(randoms, strNum/2, false));

	{
		std::unordered_map<std::string, uint32_t> strCounts;
		{
			timeTracker timmer("unordered_map", false);
			for(const auto & str : randoms){
				++strCounts[str];
			}
			setUp.logging_ <<"unordered_map\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
					<< std::endl;
		}
		{
			timeTracker timmer("unordered_map", false);
			for(const auto & str : randomsMix){
				strCounts[str];
			}
			setUp.logging_ <<"unordered_mapAccess\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
					<< std::endl;
		}
		if(setUp.verbose_){
			for(const auto & codon : strCounts){
				std::cout << codon.first << "\t" << codon.second << std::endl;
			}
		}
	}
	{
		std::map<std::string, uint32_t> strCounts;
		{
			timeTracker timmer("map", false);
			for(const auto & str : randoms){
				++strCounts[str];
			}
			setUp.logging_ <<"map\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
					<< std::endl;
		}
		{
			timeTracker timmer("map", false);
			for(const auto & str : randomsMix){
				strCounts[str];
			}
			setUp.logging_ <<"mapAccess\t"
					<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
					<< std::endl;
		}
		if(setUp.verbose_){
			for(const auto & codon : strCounts){
				std::cout << codon.first << "\t" << codon.second << std::endl;
			}
		}
	}
	return 0;
}

class infoLogging {
public:
	infoLogging(const std::string & logFilName){

	}


};

int mapVsUnorderedMapRepeat(MapStrStr inputCommands) {
	uint32_t maxSize = 50;
	uint32_t minSize = 50;
	uint32_t strNum = 50;
	uint32_t repeatNumber = 4;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(maxSize, "-maxSize", "maxSize");
	if(!setUp.setOption(minSize, "-minSize", "minSize")){
		minSize = maxSize;
	}

	if(minSize > maxSize){
		std::cout << "minSize can't be larger than maxSize, setting minSize to maxSize" << std::endl;
		minSize = maxSize;
	}
	setUp.setOption(repeatNumber, "-repeatNumber", "repeatNumber");
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "minSize: " << minSize << std::endl;
		std::cout << "maxSize: " << maxSize << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "repeatNumber: " << repeatNumber << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	setUp.extraInfo_.emplace_back("minSize", to_string(minSize));
	setUp.extraInfo_.emplace_back("maxSize", to_string(maxSize));
	setUp.extraInfo_.emplace_back("strNum", to_string(strNum));
	setUp.extraInfo_.emplace_back("repeatNumber", to_string(repeatNumber));

	randomGenerator gen;
	if(setUp.header_){
		setUp.logging_ << "mapType\t"
									<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
									<< std::endl;
	}

	VecStr randomsPreset = evenRandStrsRandLen(minSize,maxSize,
			alphabet, alphCounts,  gen, strNum/repeatNumber);
	VecStr randoms;
	randoms.reserve(strNum);
	for(uint32_t i = 0; i < repeatNumber; ++i){
		addOtherVec(randoms, randomsPreset);
	}
	std::shuffle(randoms.begin(), randoms.end(), gen.mtGen_);

	VecStr randomsMix = evenRandStrsRandLen(minSize,maxSize,
			alphabet, alphCounts,  gen, strNum/2);
	addOtherVec(randomsMix, gen.unifRandSelectionVec(randoms, strNum/2, false));

	{
		std::unordered_map<std::string, uint32_t> strCounts;
		{
			timeTracker timmer("unordered_map", false);
			for(const auto & str : randoms){
				++strCounts[str];
			}
			setUp.logging_ << "unordered_map\t"
												<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
												<< std::endl;

		}
		{
			timeTracker timmer("unordered_map", false);
			for(const auto & str : randomsMix){
				strCounts[str];
			}
			setUp.logging_ << "unordered_mapAccess\t"
												<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
												<< std::endl;

		}
		if(setUp.verbose_){
			for(const auto & codon : strCounts){
				std::cout << codon.first << "\t" << codon.second << std::endl;
			}
		}
	}
	{
		std::map<std::string, uint32_t> strCounts;
		{
			timeTracker timmer("map", false);
			for(const auto & str : randoms){
				++strCounts[str];
			}
			setUp.logging_ << "map\t"
												<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
												<< std::endl;

		}
		{
			timeTracker timmer("map", false);
			for(const auto & str : randomsMix){
				strCounts[str];
			}
			setUp.logging_ << "mapAccess\t"
												<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
												<< std::endl;

		}
		if(setUp.verbose_){
			for(const auto & codon : strCounts){
				std::cout << codon.first << "\t" << codon.second << std::endl;
			}
		}
	}

	return 0;
}

int mapVsUnorderedMapCodon(MapStrStr inputCommands) {
	uint32_t len = 51;
	uint32_t strNum = 50;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(len, "-len,-strLen", "len");
	while (len % 3 != 0){
		//just so length doesn't have to be worried about
		++len;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "len: " << len << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	setUp.extraInfo_.emplace_back("len", to_string(len));
	setUp.extraInfo_.emplace_back("strNum", to_string(strNum));
	if(setUp.header_){
		setUp.logging_ <<"mapType\t"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}

	VecStr randoms = evenRandStrs(len,
			alphabet, alphCounts,  gen, strNum);

	{
		timeTracker timmer("unordered_map", false);
		std::unordered_map<std::string, uint32_t> codonCounts;
		for(const auto & str : randoms){
			for(const auto & pos : iter::range<uint64_t>(0, str.size(), 3)){
				++codonCounts[str.substr(pos, 3)];
			}
		}
		setUp.logging_ <<"unordered_map\t"
				<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
				<< std::endl;
		timmer.reset();
		for(const auto & str : randoms){
			for(const auto & pos : iter::range<uint64_t>(0, str.size(), 3)){
				codonCounts[str.substr(pos, 3)];
			}
		}
		setUp.logging_ <<"unordered_mapAccess\t"
				<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
				<< std::endl;
		if(setUp.verbose_){
			for(const auto & codon : codonCounts){
				std::cout << codon.first << "\t" << codon.second << std::endl;
			}
		}
	}
	{
		timeTracker timmer("map", false);
		std::map<std::string, uint32_t> codonCounts;
		for(const auto & str : randoms){
			for(const auto & pos : iter::range<uint64_t>(0, str.size(), 3)){
				++codonCounts[str.substr(pos, 3)];
			}
		}

		setUp.logging_ <<"mapAccess\t"
						<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
						<< std::endl;
		timmer.reset();
		for(const auto & str : randoms){
			for(const auto & pos : iter::range<uint64_t>(0, str.size(), 3)){
				codonCounts[str.substr(pos, 3)];
			}
		}
		setUp.logging_ <<"map\t"
						<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
						<< std::endl;
		if(setUp.verbose_){
			for(const auto & codon : codonCounts){
				std::cout << codon.first << "\t" << codon.second << std::endl;
			}
		}
	}
	return 0;
}


const static std::unordered_map<std::string, char> dnaCodonToAminoAcid = {{"GCA", 'A'},
                                                                {"GCC", 'A'},
                                                                {"GCG", 'A'},
                                                                {"GCT", 'A'},
                                                                {"CGA", 'R'},
                                                                {"CGC", 'R'},
                                                                {"CGG", 'R'},
                                                                {"CGT", 'R'},
                                                                {"AGA", 'R'},
                                                                {"AGG", 'R'},
                                                                {"AAC", 'N'},
                                                                {"AAT", 'N'},
                                                                {"GAC", 'D'},
                                                                {"GAT", 'D'},
                                                                {"TGC", 'C'},
                                                                {"TGT", 'C'},
                                                                {"CAA", 'Q'},
                                                                {"CAG", 'Q'},
                                                                {"GAA", 'E'},
                                                                {"GAG", 'E'},
                                                                {"GGA", 'G'},
                                                                {"GGC", 'G'},
                                                                {"GGG", 'G'},
                                                                {"GGT", 'G'},
                                                                {"CAC", 'H'},
                                                                {"CAT", 'H'},
                                                                {"ATA", 'I'},
                                                                {"ATC", 'I'},
                                                                {"ATT", 'I'},
                                                                {"CTA", 'L'},
                                                                {"CTC", 'L'},
                                                                {"CTG", 'L'},
                                                                {"CTT", 'L'},
                                                                {"TTA", 'L'},
                                                                {"TTG", 'L'},
                                                                {"AAA", 'K'},
                                                                {"AAG", 'K'},
                                                                {"ATG", 'M'},
                                                                {"TTC", 'F'},
                                                                {"TTT", 'F'},
                                                                {"CCA", 'P'},
                                                                {"CCC", 'P'},
                                                                {"CCG", 'P'},
                                                                {"CCT", 'P'},
                                                                {"TCA", 'S'},
                                                                {"TCC", 'S'},
                                                                {"TCG", 'S'},
                                                                {"TCT", 'S'},
                                                                {"AGC", 'S'},
                                                                {"AGT", 'S'},
                                                                {"ACA", 'T'},
                                                                {"ACC", 'T'},
                                                                {"ACG", 'T'},
                                                                {"ACT", 'T'},
                                                                {"TGG", 'W'},
                                                                {"TAC", 'Y'},
                                                                {"TAT", 'Y'},
                                                                {"GTA", 'V'},
                                                                {"GTC", 'V'},
                                                                {"GTG", 'V'},
                                                                {"GTT", 'V'},
                                                                {"TAA", '*'},
                                                                {"TAG", '*'},
                                                                {"TGA", '*'}};
std::string convertToProteinWithMapNoCheck(const std::string &seq, size_t start,
                                      bool forceStartM) {
  size_t numChar = seq.size();
  std::string outSeq("");
  outSeq.resize(numChar / 3);  // numChar is exactly divisible by 3.
  std::string cBstring = "   ";
	//check to see if stop is a divisible by three to prevent trying to count a codon less than three
	uint64_t stop = seq.size();
	while ((stop - start) % 3 != 0){
		--stop;
	}
	for(const auto & pos : iter::range<uint64_t> (start, stop, 3)){
		cBstring = seq.substr(pos, 3);
		outSeq[(pos - start) / 3] = (dnaCodonToAminoAcid.at(cBstring));
		if (forceStartM && (pos == start)) {
			// Below are all the known start codons.
			if ((cBstring == "ATG") || (cBstring == "GTG") || (cBstring == "TTG") ||
					(cBstring == "ATT") || (cBstring == "CTG")) {
				outSeq[(pos - start) / 3] = 'M';
			}
		}
	}
  return outSeq;
}
std::string convertToProteinWithMap(const std::string &seq, size_t start,
                                      bool forceStartM) {
  size_t numChar = seq.size();
  std::string outSeq("");
  outSeq.resize(numChar / 3);  // numChar is exactly divisible by 3.
  std::string cBstring = "   ";
	//check to see if stop is a divisible by three to prevent trying to count a codon less than three
	uint64_t stop = seq.size();
	while ((stop - start) % 3 != 0){
		--stop;
	}
	for(const auto & pos : iter::range<uint64_t> (start, stop, 3)){
		cBstring = seq.substr(pos, 3);
		auto amino = dnaCodonToAminoAcid.find(cBstring);

		if(amino != dnaCodonToAminoAcid.end()){
			outSeq[(pos - start) / 3] = amino->second;
		}else{
			outSeq[(pos - start) / 3] = '1';
		}

		if (forceStartM && (pos == start)) {
			// Below are all the known start codons.
			if ((cBstring == "ATG") || (cBstring == "GTG") || (cBstring == "TTG") ||
					(cBstring == "ATT") || (cBstring == "CTG")) {
				outSeq[(pos - start) / 3] = 'M';
			}
		}
	}
  return outSeq;
}


std::string convertToProtein(const std::string &seq, size_t start,
                                      bool forceStartM) {
  size_t numChar = seq.size();
  std::string outSeq("");
  outSeq.resize(numChar / 3);  // numChar is exactly divisible by 3.
  // In below, cB is currentBase.
  char cB[3], newBase;
  std::string cBstring = "   ";
  for (size_t i = start; i < numChar; i += 3) {
    cB[0] = static_cast<char>(toupper(seq[i]));
    cB[1] = static_cast<char>(toupper(seq[i + 1]));
    cB[2] = static_cast<char>(toupper(seq[i + 2]));
    if (cB[0] == 'T') {
      cB[0] = 'U';
    }
    if (cB[1] == 'T') {
      cB[1] = 'U';
    }
    if (cB[2] == 'T') {
      cB[2] = 'U';
    }
    newBase = '0';  // We'll designate * as Stop,
    // and 1 as error.
    if (cB[0] == 'U') {
      if (cB[1] == 'U') {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'F';
        } else {
          newBase = 'L';
        }
      } else if (cB[1] == 'C') {
        newBase = 'S';
      } else if (cB[1] == 'A') {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'Y';
        } else {
          newBase = '*';
        }
      } else  // cB[1] == 'G'
      {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'C';
        } else if (cB[2] == 'A') {
          newBase = '*';
        } else {
          newBase = 'W';
        }
      }
    } else if (cB[0] == 'C') {
      if (cB[1] == 'U') {
        newBase = 'L';
      } else if (cB[1] == 'C') {
        newBase = 'P';
      } else if (cB[1] == 'A') {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'H';
        } else if (cB[2] == 'A') {
          newBase = 'Q';
        } else {
          newBase = 'Q';
        }
      } else  // cB[1] == 'G'
      {
        newBase = 'R';
      }
    } else if (cB[0] == 'A') {
      if (cB[1] == 'U') {
        if (cB[2] == 'G') {
          newBase = 'M';
        } else {
          newBase = 'I';
        }
      } else if (cB[1] == 'C') {
        newBase = 'T';
      } else if (cB[1] == 'A') {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'N';
        } else {
          newBase = 'K';
        }
      } else  // cB[1] == 'G'
      {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'S';
        } else {
          newBase = 'R';
        }
      }
    } else  // cb[0] == 'G'
    {
      if (cB[1] == 'U') {
        newBase = 'V';
      } else if (cB[1] == 'C') {
        newBase = 'A';
      } else if (cB[1] == 'A') {
        if (cB[2] == 'U' || cB[2] == 'C') {
          newBase = 'D';
        } else {
          newBase = 'E';
        }
      } else  // cB[1] == 'G'
      {
        newBase = 'G';
      }
    }

    if (forceStartM && (i == start)) {
      cBstring[0] = cB[0];
      cBstring[1] = cB[1];
      cBstring[2] = cB[2];
      // Below are all the known start codons.
      if ((cBstring == "AUG") || (cBstring == "GUG") || (cBstring == "UUG") ||
          (cBstring == "AUU") || (cBstring == "CUG")) {
        newBase = 'M';
      }
    }
    outSeq[(i - start) / 3] = newBase;
  }
  return outSeq;
}



int translation(MapStrStr inputCommands) {
	uint32_t len = 51;
	uint32_t strNum = 50;
	std::string alphStr = "A,C,G,T";
	std::string alphDelim = ",";
	bool veryVerbose = false;
	profilerSetUp setUp(inputCommands);
	setUp.setOption(len, "-len,-strLen", "len");
	while (len % 3 != 0){
		//just so length doesn't have to be worried about
		++len;
	}
	setUp.setOption(strNum, "-strNum", "strNum");
	setUp.setOption(alphStr, "-alphStr", "alphStr");
	setUp.setOption(alphDelim, "-alphDelim", "alphDelim");
	setUp.setOption(veryVerbose, "-veryVerbose,-vv", "veryVerbose");
	setUp.finishSetUp(std::cout);
	auto processAlph = processAlphStrVecCharCounts(alphStr, alphDelim);
	std::vector<char> alphabet = processAlph.first;
	std::vector<uint32_t> alphCounts = processAlph.second;
	if(setUp.verbose_){
		std::cout << "len: " << len << std::endl;
		std::cout << "strNum: " << strNum << std::endl;
		std::cout << "alphStr: " << alphStr << std::endl;
		std::cout << "alphDelim: " << alphDelim << std::endl;
		std::cout << "alphabet: " << vectorToString(alphabet, ", ") << std::endl;
		std::cout << "alphCounts: " << vectorToString(alphCounts, ", ") << std::endl;
	}
	randomGenerator gen;
	setUp.extraInfo_.emplace_back("len", to_string(len));
	setUp.extraInfo_.emplace_back("strNum", to_string(strNum));
	if(setUp.header_){
		setUp.logging_ <<"mapType\t"
				<< getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_)
				<< std::endl;
	}

	VecStr randoms = evenRandStrs(len,
			alphabet, alphCounts,  gen, strNum);

	//check to see if translation is the same
	std::string translated1 = convertToProtein(randoms.front(), 0, false);
	std::string translated2 = convertToProteinWithMap(randoms.front(), 0, false);
	std::string translated3 = convertToProteinWithMapNoCheck(randoms.front(), 0, false);
	if(translated1 != translated2){
		std::cout << "translated1 does not equal translated2" << std::endl;
		std::cout << translated1 << std::endl;
		std::cout << translated2 << std::endl;
		exit(1);
	}
	if(translated1 != translated3){
		std::cout << "translated1 does not equal translated3" << std::endl;
		std::cout << translated1 << std::endl;
		std::cout << translated3 << std::endl;
		exit(1);
	}
	{
		timeTracker timmer("oldWay", false);
		std::string translated = ""	;
		for(const auto & str : randoms){
			translated = convertToProtein(str, 0, false);
		}
		setUp.logging_ <<"oldWay\t"
				<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
				<< std::endl;
	}
	{
		timeTracker timmer("map", false);
		std::string translated = ""	;
		for(const auto & str : randoms){
			translated = convertToProteinWithMap(str, 0, false);
		}
		setUp.logging_ <<"map\t"
				<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
				<< std::endl;
	}
	{
		timeTracker timmer("mapNoCheck", false);
		std::string translated = ""	;
		for(const auto & str : randoms){
			translated = convertToProteinWithMapNoCheck(str, 0, false);
		}
		setUp.logging_ <<"mapNoCheck\t"
				<< getRunInfo("\t", false, setUp.extraInfo_, timmer)
				<< std::endl;
	}
	return 0;
}

void f1(randomGenerator & gen, std::vector<double> & row){
	for(const auto & pos : iter::range(row.size())){
		row[pos] = gen();
	}
}

int testThreads(MapStrStr inputCommands) {
	profilerSetUp setUp(inputCommands);
	uint32_t numThreads = 2;
	uint32_t rowNum = 1000;
	uint32_t colNum = 10;
	setUp.setOption(numThreads, "-threads,-numThreads", "numThreads");
	setUp.setOption(rowNum, "-rowNum", "rowNum");
	setUp.setOption(colNum, "-colNum", "colNum");
	setUp.finishSetUp(std::cout);
	setUp.extraInfo_.emplace_back("numThreads", to_string(numThreads));
	setUp.extraInfo_.emplace_back("rowNum", to_string(rowNum));
	setUp.extraInfo_.emplace_back("colNum", to_string(colNum));
	if(setUp.header_){
		setUp.logging_ << "runType\t" << getRunInfo("\t", true, setUp.extraInfo_, setUp.timer_) << std::endl;
	}
  {
	  std::vector<randomGenerator> gens(numThreads);
	  std::vector<std::vector<double>> randomNums (rowNum, std::vector<double>(colNum));
  	timeTracker timmer("threaded", false);
    for(const auto & currentRow : iter::range<uint64_t>(0, randomNums.size(), numThreads)){
    	std::vector<std::thread> threads;
			for(const auto & i : iter::range(numThreads)){
				if((currentRow + i) < randomNums.size()){
					threads.emplace_back(std::thread(f1, std::ref(gens[i]), std::ref(randomNums[currentRow + i])));
				}
			}
			for(auto & t : threads){
				t.join();
			}
    }
    setUp.logging_ << "threaded\t" << getRunInfo("\t", false, setUp.extraInfo_, timmer)<< std::endl;
  }
  {
    std::vector<randomGenerator> gens(numThreads);
    std::vector<std::vector<double>> randomNums (rowNum, std::vector<double>(colNum));
  	timeTracker timmer("non-threaded", false);
    uint32_t currentRow = 0;
    while(currentRow < randomNums.size()){
      for(const auto & i : iter::range(numThreads)){
      	if(currentRow >= randomNums.size()){
      		break;
      	}
      	f1(gens[i], randomNums[currentRow]);
      	++currentRow;
      }
    }
    setUp.logging_ << "non-threaded\t" << getRunInfo("\t", false, setUp.extraInfo_, timmer)<< std::endl;
  }
	return 0;
}
/* profiler template
 * int nameOfProgram(MapStrStr inputCommands) {
 *  //programSetUp for easy command line parsing
 * 	programSetUp setUp(inputCommands);
 * 	T variable1 = defaultValue;
 * 	T2 variable2 = defaultValue2
 * 	//setOption takes four options variableToSet, multipleFlags_commandDelimited, nameOfVariable, requried(byDeault false)
 * 	//setOption is also overloaded to set more of the common std libary variable, int types (32_t, 64_t etc), double, std::string etc
 *	setUp.setOption(variable1, "-variable1", "variable1");
 *  setUp.setOption(variable2, "-variable2,-var2", "variable2", true);
 *  //always finish with setUp.finishSetUp(std::cout); so you
 *  //can use -getFlags and have the setUp print warnings
 *  setUp.finishSetUp(std::cout);
 *
 *  //your code here, compilerUsed is a static variable set to clang or gcc so you can use to log which compiler you are using
 *  //compilerProfiler comes with a random number generator, functions for generating random strings, an aligner, a timer to log run times
 *  //can include own code to profile
 *  // once you write your own profiler, just add to profilerRunner with the addFunc function in the initialization seen bellow
 *  // which takes name to call the program at commandline, the function itself, and whether the name is an alias and
 *  // shouldn't be printed to commandline, and whether to make the call lower case to make the calling case insensitive (default)
 *
 * return 0;
 * }
 */

profilerRunner::profilerRunner()
    : programRunner(
          {addFunc("fullAlignmentProfiler", fullAlignmentProfiler, false),
					 addFunc("simpleAlignmentProfiler", simpleAlignmentProfiler, false),
					 addFunc("justScoreAlignmentProfiler", justScoreAlignmentProfiler, false),
					 addFunc("randomNumberGeneration", randomNumberGeneration, false),
					 addFunc("mapVsUnorderedMapCodon", mapVsUnorderedMapCodon, false),
					 addFunc("mapVsUnorderedMap", mapVsUnorderedMap, false),
					 addFunc("mapVsUnorderedMapRepeat", mapVsUnorderedMapRepeat, false),
					 addFunc("translation", translation, false),
					 addFunc("testThreads", testThreads, false)
           },
          "profilerRunner") {}

int main(int argc, char* argv[]) {
	//std::cout << __VERSION__ << std::endl;
	//std::cout << getCompilerVersion() << std::endl;
	//std::cout << optimizationLevel << std::endl;
	//std::cout << processorInfo << std::endl;
	//std::cout << TESTMACRO << std::endl;
  profilerRunner proRunner;
  if (argc > 1) {
    return proRunner.run(argc, argv);
  }
  proRunner.listPrograms(std::cout);
  return 0;
}
