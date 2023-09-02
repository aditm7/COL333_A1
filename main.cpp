#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

#include "SL.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv )
{

     // Parse the input.
    if ( argc < 3 )
    {   
        cout<<"Missing arguments\n";
        cout<<"Correct format : \n";
        cout << "./main <input_filename> <output_filename>";
        exit ( 0 );
    }
    string inputfilename ( argv[1] );
    string outputfilename ( argv[2] );
    auto start = high_resolution_clock::now();

    SportsLayout *s  = new SportsLayout( inputfilename );
    s->compute_allocation(s->find_best_mapping());
    s->write_to_file(outputfilename);
    long long cost = s->cost_fn (s->mapping);
    cout<< "cost:"<<cost<<endl;

    bool flag=true;
    set<int> locations_mapped(s->mapping,s->mapping+s->z);
    flag &= locations_mapped.size()==s->z;
    if(!flag) cout<<"non-distinct values in the output!!"<<endl;
    else cout<<"correct output format"<<endl;
    
    auto stop = high_resolution_clock::now();auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time taken: "<< duration.count() << " ms" << endl;
    return 0;

}