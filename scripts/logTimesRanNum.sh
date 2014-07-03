#!/bin/bash

#logTimes=5

if [ ! -d profiles ]; then
	mkdir profiles
fi 


if [ -f profiles/holdTimesRanNum.txt ]; then
	bin/clangProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
else 
	bin/clangProfiler randomNumberGeneration -header "$@" >> profiles/holdTimesRanNum.txt
fi


bin/gxxProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/clangProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/gxxProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/clangProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/gxxProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/clangProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/gxxProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/clangProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
bin/gxxProfiler randomNumberGeneration "$@" >> profiles/holdTimesRanNum.txt
