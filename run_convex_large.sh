#!/bin/bash

# Check number of arguments
if [ $# -ne 2 ]; then
  echo "Usage: $0 <script1> <script2>"
  echo "You provided $# argument(s)."
  exit 1
fi

scripts="$1 $2"

# Store folder names in a variable (space-separated)
inputs="700 800 900 1000"

# Loop through each script and input file
for script in $scripts; do
  for input in $inputs; do
    python3 ${script}.py < ./Convex/large_inputs/input_${input} | tail -n 1 >> ./Convex/results_${script}_large_input.txt
  done
done