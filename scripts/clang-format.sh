#!/bin/bash

DIRS="src"
if [ $(uname) == "Linux" ]; then
  find $DIRS -regex ".*\.[cChH]\(pp\)?" | xargs clang-format-3.4 -i --style=Google
else
  gfind $DIRS -regex ".*\.[cChH]\(pp\)?" | xargs clang-format -i --style=Google
fi

