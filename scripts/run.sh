#!/usr/bin/env zsh
set -e

CUR_DIR="$(dirname $0)"

# Configure the time command's output format.
export TIMEFMT='%J   %U  user %S system %P cpu %*E total'$'\n'\
'avg shared (code):         %X KB'$'\n'\
'avg unshared (data/stack): %D KB'$'\n'\
'total (sum):               %K KB'$'\n'\
'max memory:                %M MB'$'\n'\
'page faults from disk:     %F'$'\n'\
'other page faults:         %R'

# Build
$CUR_DIR/build.sh
time ./app 2>&1 | bat -llog -P
