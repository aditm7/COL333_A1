#!/bin/zsh

# Define a function to run your C++ program
run_cpp() {
    param="$1"
    output_file="./results/out_${param}.txt"
    ./main1.cpp -iterations $param > $output_file
}

# Export the function for parallel execution
export -f run_cpp

# Use parallel to run processes in parallel
seq 500 500 4000 | parallel run_cpp {}

# Remove export
unset -f run_cpp
