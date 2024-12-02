#!/usr/bin/env bash
# Monitors and renders markdown & latex files.

SCRIPT_DIR="$(dirname $0)"
PDF_FILEPATH="$1"
[ -e "$PDF_FILEPATH" ] || { echo "File does not exist!"; exit 0; }

# Extract and lowercase the file extension
FILE_EXTENSION="${PDF_FILEPATH##*.}"
FILE_EXTENSION="$(echo $FILE_EXTENSION | tr '[:upper:]' '[:lower:]')"

FILENAME_NO_EXT="$(basename ${PDF_FILEPATH%.*})"

# Cleans up running jobs after script termination.
function cleanup() {
  if [[ $PDF_PID != "" ]]; then
    echo "Terminating pdf pid: $PDF_PID"
    kill -TERM $PDF_PID
    echo "Waiting on jobs..."
  fi
  wait
}
trap cleanup SIGINT EXIT

if [[ "$FILE_EXTENSION" != "tex" ]]; then
  echo "Unsupported file extension '$FILE_EXTENSION'"
  exit 0
fi

while [ 1 ]; do
  # Don't block on failure.
  pdflatex -interaction=nonstopmode $PDF_FILEPATH

  # Spawn pdf viewer app if not running.
  if [[ $PDF_PID == "" ]]; then
    okular "${FILENAME_NO_EXT}.pdf"&
    PDF_PID="$!"
  fi

  # Exit on sigint.
  inotifywait -e close_write "$PDF_FILEPATH" || exit 0
  clear
done

