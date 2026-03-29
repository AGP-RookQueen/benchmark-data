#!/bin/bash


# Check number of arguments
if [ $# -lt 1 ]; then
  echo "Usage: $0 <script name>"
  echo "You provided $# argument(s)."
  exit 1
fi

script=$1

# Store folder names in a variable (space-separated)
folders="Convex Generic Paralellogram"

MAX=400

# Loop through each folder name
for folder in $folders; do
    rm ${folder}/results_${1}.txt
    touch ${folder}/results_${1}.txt
    for ((i = 4 ; i <= MAX ; i += 2 )); do
        echo "Running on test: $i"
        echo -n "Test $i: " >> ${folder}/results_${1}.txt
        python3 ${1}.py < ${folder}/input/input_${i} | tail -n 1 >> ${folder}/results_${1}.txt
    done

done