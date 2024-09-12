#/usr/bin/env bash
PROJECT_DIR="$(realpath $(dirname $0)/..)"
cd $PROJECT_DIR

cppcheck \
  -I ./includes \
  -I ./vendors/spdlog/include \
  -I ./ \
  --error-exitcode=1 \
  --enable=all \
  --suppress=missingIncludeSystem \
  --suppress=checkersReport \
  --suppress=unusedFunction \
  --suppress=unmatchedSuppression \
  --language=c++ \
  --check-level=exhaustive \
  --std=c++20 \
  ./src || exit 1

