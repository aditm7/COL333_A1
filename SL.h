#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
extern mt19937 gen;

struct SportsLayout{
    int z,l;
    int** T;
    int **N;
    double time;
    int *mapping;
    int it;
    chrono::high_resolution_clock::time_point start;
    
    SportsLayout(string inputfilename);
    
    // declare the destructor here
    ~SportsLayout();

    bool check_output_format();
    
    long long cost_fn(int* mp);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation(int* mp);
    
    long long find_contribution(int* mp,int idx);
    
    bool exit_indicator();
    
    int* generate_random_mapping();

    int* find_best_mapping();
    
    void greedy_with_restarts(int* best_mp,long long &best_cost);
    
    void simulated_annealing(int* best_mp,int &best_cost);

};
#endif