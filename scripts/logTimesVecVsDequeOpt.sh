#!/bin/bash


if [ ! -d profiles ]; then
	mkdir profiles
fi 

runTimes=4
lens=($(seq -f %f 100000 100000 10000000))
for l in "${lens[@]}" ; do
	for i in $(seq 1 $runTimes); do 
		gxxProfiler vectorVsDeque -stop $l  -logFilename profiles/vecVsDequeOpt.txt
		clangProfiler vectorVsDeque -stop $l  -logFilename profiles/vecVsDequeOpt.txt
	done
done



