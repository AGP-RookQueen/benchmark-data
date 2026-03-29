#!/bin/bash

# Check number of arguments
if [ $# -lt 2 ]; then
  echo "Usage: $0 <script name 1> <script name 1>"
  echo "You provided $# argument(s)."
  exit 1
fi

scripts="$1 $2"

# Store folder names in a variable (space-separated)
folders="Convex Generic Paralellogram"

MAX=400

# Loop through each folder name
for script in $scripts; do
  for folder in $folders; do
      touch ${folder}/results_build_${script}_final.txt
      for ((i = 4 ; i <= MAX ; i += 2 )); do
          echo "Running on test: $i"
          echo -n "Test $i: " >> ${folder}/results_build_${script}_final.txt
          python3 ${script}.py < ${folder}/input/input_${i} | tail -n 1 >> ${folder}/results_build_${script}_final.txt
      done
  done
done