#include "SL.h"

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
    auto main_start = high_resolution_clock::now();

    SportsLayout *s  = new SportsLayout( inputfilename );
    s->start = high_resolution_clock::now();

    s->compute_allocation(s->find_best_mapping());
    s->write_to_file(outputfilename);
    long long cost = s->cost_fn (s->mapping);
    cout<< "cost:"<<cost<<endl;

    s->check_output_format();
    
    auto main_stop = high_resolution_clock::now();auto main_duration = duration_cast<milliseconds>(main_stop - main_start);
    cout << "Time taken: "<< main_duration.count() << " ms" << endl;
    return 0;

}