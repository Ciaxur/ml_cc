#/usr/bin/env bash
CUR_DIR="$(dirname $0)"

cppcheck \
  -I $CUR_DIR \
  --error-exitcode=1 \
  --enable=all \
  --suppress=missingIncludeSystem \
  --suppress=checkersReport \
  --suppress=unusedFunction \
  --suppress=unmatchedSuppression \
  --language=c++ \
  --check-level=exhaustive \
  --std=c++20 \
  "$@" || exit 1

