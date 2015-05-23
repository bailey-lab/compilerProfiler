#!/bin/bash

if [ ! -d profiles ]; then
	mkdir profiles
fi 
runTimes=4
lens=(1000 2000 3000 4000 5000 6000 7000 8000 9000 10000)
repeats=(10 20 50 100 200)
for l in "${lens[@]}" ; do
	for r in "${repeats[@]}" ; do
		for i in $(seq 1 $runTimes); do 
			gxxProfilerNoOpt mapVsUnorderedMapRepeat -repeatNumber $r -strNum 10000 -minSize $l -maxSize $l -log profiles/logMapRepeatTimes.txt
			gxxProfiler mapVsUnorderedMapRepeat -repeatNumber $r  -strNum 10000 -minSize $l -maxSize $l -log profiles/logMapRepeatTimes.txt
			clangProfilerNoOpt mapVsUnorderedMapRepeat -repeatNumber $r  -strNum 10000 -minSize $l -maxSize $l -log profiles/logMapRepeatTimes.txt
			clangProfiler mapVsUnorderedMapRepeat -repeatNumber $r  -strNum 10000 -minSize $l -maxSize $l -log profiles/logMapRepeatTimes.txt
		done
	done
done