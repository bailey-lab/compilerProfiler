#!/bin/bash
logTimes=5

if [ ! -d profiles ]; then
	mkdir profiles
fi 

lens=(50 100 150 200 250 300 350 400 450 500 550 600 650)

for l in "${lens[@]}" ; do
	for i in $(seq 1 $logTimes); do 
		gxxProfiler fullAlignmentProfiler -maxSize $l -minSize $l -log profiles/alnTimesVarLen.txt
		clangProfiler fullAlignmentProfiler -maxSize $l -minSize $l -log profiles/alnTimesVarLen.txt
		macClangProfiler fullAlignmentProfiler -maxSize $l -minSize $l -log profiles/alnTimesVarLen.txt
	done
done
