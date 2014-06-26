#!/bin/bash

#logTimes=5
if [ ! -d profiles ]; then
	mkdir profiles
fi 

bin/clangProfiler fullAlignmentProfiler -header "$@" > profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/clangProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
bin/gxxProfiler fullAlignmentProfiler "$@" >> profiles/holdTimes.txt
