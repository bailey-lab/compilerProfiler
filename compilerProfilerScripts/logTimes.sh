#!/bin/bash

#logTimes=5

if [ ! -d profiles ]; then
	mkdir profiles
fi 


if [ -f profiles/holdTimes.txt ]; then
	bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
else 
	bin/clangProfiler fullAlignmentProfiler -header "$@" >> profiles/holdTimes.txt
fi


bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
