#!/bin/bash

list() (
  cd "$1"
  for i in *; do
    if [ -f "$i" ]; then
      echo "$PWD/$i"
    elif [ -d "$i" ]; then
      #echo "dir: $PWD/$i"
      list "$i"
    fi
  done
)

 
allFiles=$(echo $(list $1) | tr ' ' '\n')
for x in $allFiles
do 
	cat $2 >> $x
done