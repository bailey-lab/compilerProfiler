#!/bin/bash


if [ ! -d profiles ]; then
	mkdir profiles
fi 

runTimes=4
lens=$(seq 100000 1000 10000000)
for l in "${lens[@]}" ; do
	for i in $(seq 1 $runTimes); do 
		
		gxxProfilerNoOpt vectorVsDeque -stop $l  -logFilename profiles/vecVsDeque.txt
		gxxProfiler vectorVsDeque -stop $l  -logFilename profiles/vecVsDeque.txt
		clangProfilerNoOpt vectorVsDeque -stop $l -logFilename profiles/vecVsDeque.txt
		clangProfiler vectorVsDeque -stop $l  -logFilename profiles/vecVsDeque.txt
	done
done



