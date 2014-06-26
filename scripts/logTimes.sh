#!/bin/bash

#logTimes=5
if [ ! -d profiles ]; then
	mkdir profiles
fi 

bin/clangAlignTest -header "$@" > profiles/holdTimes.txt
bin/gxxAlignTest "$@" >> profiles/holdTimes.txt
bin/clangAlignTest "$@" >> profiles/holdTimes.txt
bin/gxxAlignTest "$@" >> profiles/holdTimes.txt
bin/clangAlignTest "$@" >> profiles/holdTimes.txt
bin/gxxAlignTest "$@" >> profiles/holdTimes.txt
bin/clangAlignTest "$@" >> profiles/holdTimes.txt
bin/gxxAlignTest "$@" >> profiles/holdTimes.txt
bin/clangAlignTest "$@" >> profiles/holdTimes.txt
bin/gxxAlignTest "$@" >> profiles/holdTimes.txt
