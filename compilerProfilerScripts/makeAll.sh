#!/bin/bash

for i in compilerMakeInfos/*; do
	make COMPFILE=$i "$@"
done

