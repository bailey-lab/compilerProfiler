#!/bin/bash

if [ ! -d profiles ]; then
	mkdir profiles
fi 
runTimes=4
lens=(100 200 300 400 500 600 700 800 900)
gxxProfilerNoOpt mapVsUnorderedMap -strNum 100000 -minSize 50 -maxSize 50 -header > profiles/logMapTimes.txt
for i in $(seq 1 3); do 
	gxxProfilerNoOpt mapVsUnorderedMap -strNum 100000 -minSize 50 -maxSize 50 >> profiles/logMapTimes.txt
done
for i in $(seq 1 $runTimes); do 
	gxxProfiler mapVsUnorderedMap -strNum 100000 -minSize 50 -maxSize 50 >> profiles/logMapTimes.txt
	clangProfilerNoOpt mapVsUnorderedMap -strNum 100000 -minSize 50 -maxSize 50 >> profiles/logMapTimes.txt
	clangProfiler mapVsUnorderedMap -strNum 100000 -minSize 50 -maxSize 50 >> profiles/logMapTimes.txt
done

  
for l in "${lens[@]}" ; do
	for i in $(seq 1 $runTimes); do 
		gxxProfilerNoOpt mapVsUnorderedMap -strNum 100000 -minSize $l -maxSize $l >> profiles/logMapTimes.txt
		gxxProfiler mapVsUnorderedMap -strNum 100000 -minSize $l -maxSize $l >> profiles/logMapTimes.txt
		clangProfilerNoOpt mapVsUnorderedMap -strNum 100000 -minSize $l -maxSize $l >> profiles/logMapTimes.txt
		clangProfiler mapVsUnorderedMap -strNum 100000 -minSize $l -maxSize $l >> profiles/logMapTimes.txt
	done
done