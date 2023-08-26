
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct SportsLayout{
    int z,l;
    int** T;
    int **N;
    int time;
    int *mapping;
    
    SportsLayout(string inputfilename);

    bool check_output_format();
    
    long long cost_fn(int* mp);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation(int* mp);

    int* greedy_with_restarts();
    
    int* generate_random_mapping();

};


#endif