#!/bin/bash
run_cpp(){
    param="$1"
    output=$(./bin/main1.out ./io_files/in.txt ./bin/out_${param}.txt "$param" )
    > ./results/out_${param}.txt
    echo "$output" >> ./results/out_${param}.txt
}

export -f run_cpp
seq 500 500 8000 | parallel -j16 run_cpp {}
unset -f run_cpp
