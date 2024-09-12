#!/usr/bin/env zsh
# This script is used to vet and run code ready for submission.
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

# Check
# TODO: disabled due to checking vendors directory. once excluded, this
# can be re-enabled.
#
# echo -e "\033[35m== Running checks ==\033[0m"
# $CUR_DIR/check.sh

# Build
$CUR_DIR/build.sh

# Valgrind it
echo -e "\033[35m\n== Running valgrind ==\033[0m"
valgrind -s ./app > /dev/null

# Finally run main and time it.
echo -e "\033[35m\n== Timing main binary ==\033[0m"
time ./app
