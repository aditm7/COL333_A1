#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
mt19937 gen(steady_clock::now().time_since_epoch().count());

#include "SL.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
          
        readInInputFile(inputfilename);
        mapping= new int[z];

    }

    bool SportsLayout::check_output_format()
    {

        vector<bool> visited(l,false);
        for(int i=0;i<z;i++)
        {
            if((mapping[i]>=1 && mapping[i]<=l))
            {
                if(!visited[mapping[i]-1])
                visited[mapping[i]-1]=true;
                else
                {
                    cout<<"Repeated locations, check format\n";
                    return false;
                }
            }
            else
            {
                cout<<"Invalid location, check format\n";
                return false;
            }
        }

        return true;

    }


    long long SportsLayout::cost_fn(int* mp){
        long long cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=i+1;j<z;j++) 
           {
                cost+=((long long)N[i][j]+(long long)N[j][i])*(long long)T[mp[i]-1][mp[j]-1];
           }
        }
        return cost;
    }

    void SportsLayout::readInInputFile(string inputfilename)
    {
            fstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                

                ipfile>> time;
                ipfile >> z;
                ipfile >> l;

                if(z>l)
                {
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }


            

            int **tempT;
            int **tempN;

          tempT = new int*[l];
         for (int i = 0; i < l; ++i)
            tempT[i] = new int[l];
        
        tempN = new int*[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new int[z];

        for(int i=0;i<z;i++)
        {
            for(int j=0;j<z;j++)
            ipfile>>tempN[i][j];
        }

        for(int i=0;i<l;i++)
        {
            for(int j=0;j<l;j++)
            ipfile>>tempT[i][j];
        }

        ipfile.close();

        T= tempT;
        N= tempN;
            }

    }

    void SportsLayout::write_to_file(string outputfilename){

         // Open the file for writing
        ofstream outputFile(outputfilename);

        // Check if the file is opened successfully
        if (!outputFile.is_open()) {
            cerr << "Failed to open the file for writing." << std::endl;
            exit(0);
        }

        for(int i=0;i<z;i++)
        outputFile<<mapping[i]<<" ";

        // Close the file
        outputFile.close();

        cout << "Allocation written to the file successfully." << endl;

    }

    int* SportsLayout::greedy_with_restarts(){
        uniform_int_distribution<long long> rnd(0,INT_MAX);
        int locations[l];
        for(int i=0;i<l;i++) locations[i]=i+1;
        for(int i=0;i<l;i++) swap(locations[i],locations[rnd(gen)%l]);
        int init_mp[z];
        for(int i=0;i<z;i++) init_mp[i]=locations[i];
        for(int i=0;i<l;i++){
            cout<<locations[i]<<" ";
        }
        cout<<endl;
        int* best_mp=init_mp;
        int best_cost=cost_fn(init_mp);

        
        return init_mp;
    }

    void SportsLayout::compute_allocation(int* mp)
    {
        for(int i=0;i<z;i++)
        mapping[i]=mp[i];
    }


