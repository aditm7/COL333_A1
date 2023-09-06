#!/bin/bash

#test case file names
# test_cases="input1.txt" "input2.txt" "input6.txt" "large.txt" "in_50z_100l.txt"  "testcase_10z_20l.txt"
# lsit all input files from io_files
test_cases=(`ls ./io_files/`)
for i in "${test_cases[@]}"
do
    echo "Running test case: $i"
    #kill the process after 10 seconds
    ./main.out ./io_files/$i ./out.txt
done