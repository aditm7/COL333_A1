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
    
    int* SportsLayout::generate_random_mapping(){
        int* locations = new int[l];
        for(int i=0;i<l;i++) locations[i]=i+1;
        shuffle(locations,locations+l,gen);
        int* init_mp = new int[z];
        for(int i=0;i<z;i++) init_mp[i]=locations[i];
        return init_mp;
    }

    void SportsLayout::compute_allocation(int* mp)
    {
        for(int i=0;i<z;i++)
        mapping[i]=mp[i];
    }
    
    long long SportsLayout::find_contribution(int* mp,int idx){
        long long ans=0;
        for(int i=0;i<z;i++){
            ans+=((long long)N[i][idx]+(long long)N[idx][i])*(long long)T[mp[i]-1][mp[idx]-1];
        }
        return ans;
    }
    void SportsLayout::greedy_with_restarts(int *best_mp, int &best_cost)
    {
        auto start = high_resolution_clock::now();
        double time_var=(time*1.0*60-1)*1000;
        int *curr_mp = new int[z];
        int curr_cost, curr_best_cost = INT_MAX;
        int *curr_best_mp = new int[z];

        auto next_state = [&](vector<bool> &used_locations, set<pair<long long, int>> &contri)
        {
            int i = (*(--contri.end())).second;
            long long initial_contribution = (*(--contri.end())).first;
            int curr_location = curr_mp[i];
            int best_location = curr_location;
            int best_location_cost = curr_cost;

            for (int j = 0; j < l; j++)
            {
                if (!used_locations[j + 1])
                {
                    curr_cost -= initial_contribution;
                    curr_mp[i] = j + 1;
                    long long temp_contri = find_contribution(curr_mp, i);
                    curr_cost += temp_contri;

                    if (curr_cost < best_location_cost)
                    {
                        best_location_cost = curr_cost;
                        best_location = j + 1;
                    }
                    // rollback the changes
                    curr_cost -= temp_contri;
                    curr_cost += initial_contribution;
                    curr_mp[i] = curr_location;
                }
            }

            curr_mp[i] = best_location;
            used_locations[curr_location] = false;
            used_locations[best_location] = true;
            contri.erase(--contri.end());
            contri.insert({find_contribution(curr_mp, i), i});
            curr_cost = cost_fn(curr_mp);

            if (curr_cost < curr_best_cost)
            {
                curr_best_cost = curr_cost;
                for (int j = 0; j < z; j++)
                    curr_best_mp[j] = curr_mp[j];
            }
        };

        vector<bool> used_locations(l + 1, false);
        auto globalstop = high_resolution_clock::now();
        auto globalduration = duration_cast<milliseconds>(globalstop - start);
        while (globalduration.count() < time_var)
        {
            int *temp = generate_random_mapping();

            for (int i = 0; i < z; i++)
            {
                curr_mp[i] = temp[i];
                curr_best_mp[i] = temp[i];
            }

            curr_cost = cost_fn(curr_mp);
            curr_best_cost = curr_cost;

            used_locations.assign(l + 1, false);
            for (int i = 0; i < z; i++)
                used_locations[curr_mp[i]] = 1;

            set<pair<long long, int>> contri;
            for (int i = 0; i < z; i++)
                contri.insert({find_contribution(curr_mp, i), i});

            int iterations = 4000;

            while (iterations--)
            {
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop - start);
                if(duration.count()>=time_var)
                    return;
                next_state(used_locations, contri);
            }
                

            if (curr_best_cost < best_cost)
            {
                best_cost = curr_best_cost;
                for (int i = 0; i < z; i++)
                    best_mp[i] = curr_best_mp[i];
            }
            globalstop = high_resolution_clock::now();
            globalduration = duration_cast<milliseconds>(globalstop - start);
        }
        return;
    }


    void SportsLayout::simulated_annealing(int *best_mp, int &best_cost)
    {
        auto start = high_resolution_clock::now();
        int *curr_mp = new int[z];
        int curr_cost, curr_best_cost = INT_MAX;
        int *curr_best_mp = new int[z];
        double threshold = 0.0;
        bool rand_flag = false;
        uniform_int_distribution<int> rnd2(0, z - 1);
        auto next_state_greedy = [&](vector<bool> &used_locations, set<pair<int, int>> &contri, vector<int> &unused_locations)
        {
            int i = (*(--contri.end())).second;
            int initial_contribution = (*(--contri.end())).first;
            if (rand_flag)
            {
                i = rnd2(gen);
                initial_contribution = find_contribution(curr_mp, i);
            }
            int curr_location = curr_mp[i];
            int best_location = curr_location;
            int best_location_cost = curr_cost;
            int best_location_idx = -1;
            for (int j = 0; j < l - z; j++)
            {
                curr_cost -= initial_contribution;
                curr_mp[i] = unused_locations[j];
                int temp_contri = find_contribution(curr_mp, i);
                curr_cost += temp_contri;

                if (curr_cost < best_location_cost)
                {
                    best_location_cost = curr_cost;
                    best_location_idx = j;
                    best_location = unused_locations[j];
                }
                // rollback the changes
                curr_cost -= temp_contri;
                curr_cost += initial_contribution;
                curr_mp[i] = curr_location;
            }

            curr_mp[i] = best_location;
            used_locations[curr_location] = false;
            used_locations[best_location] = true;
            contri.erase({initial_contribution, i});
            contri.insert({find_contribution(curr_mp, i), i});
            if (best_location_idx != -1)
                unused_locations[best_location_idx] = curr_location;
            curr_cost = cost_fn(curr_mp);

            if (curr_cost < curr_best_cost)
            {
                curr_best_cost = curr_cost;
                for (int j = 0; j < z; j++)
                    curr_best_mp[j] = curr_mp[j];
            }
        };

        uniform_real_distribution<double> prob(0.0, 1.0);
        uniform_int_distribution<int> rnd3(0, l - z - 1);
        double time_var=(time*1.0*60-1)*1000;
        auto next_state_random = [&](vector<bool> &used_locations, set<pair<int, int>> &contri, vector<int> &unused_locations)
        {
            int idx = rnd2(gen);
            int loc = rnd3(gen);
            int curr_location = curr_mp[idx];
            long long initial_contribution = find_contribution(curr_mp, idx);
            curr_mp[idx] = unused_locations[loc];
            long long final_contribution = find_contribution(curr_mp, idx);

            if (final_contribution < initial_contribution)
            {
                curr_cost += final_contribution - initial_contribution;
                if (curr_cost < curr_best_cost)
                {
                    curr_best_cost = curr_cost;
                    for (int j = 0; j < z; j++)
                        curr_best_mp[j] = curr_mp[j];
                }
                contri.erase({initial_contribution, idx});
                contri.insert({final_contribution, idx});
                used_locations[curr_mp[idx]] = 1;
                unused_locations[loc] = curr_location;
            }
            else
            {
                // rollback
                curr_mp[idx] = curr_location;
            }
        };

        vector<bool> used_locations(l + 1, false);
        vector<int> unused_locations;
        auto globalstop = high_resolution_clock::now();
        auto globalduration = duration_cast<milliseconds>(globalstop - start);
        while (globalduration.count() < time_var)
        {
            int *temp = generate_random_mapping();
            threshold = 0.0;
            rand_flag = false;
            for (int i = 0; i < z; i++)
            {
                curr_mp[i] = temp[i];
                curr_best_mp[i] = temp[i];
            }

            curr_cost = cost_fn(curr_mp);
            curr_best_cost = curr_cost;

            used_locations.assign(l + 1, false);
            for (int i = 0; i < z; i++)
                used_locations[curr_mp[i]] = 1;
            unused_locations.clear();
            for (int i = 1; i <= l; i++)
                if (used_locations[i] == 0)
                    unused_locations.push_back(i);
            set<pair<int, int>> contri;
            for (int i = 0; i < z; i++)
                contri.insert({find_contribution(curr_mp, i), i});

            int iterations = 2000;
            int it = iterations;
            while (iterations--)
            {
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(stop - start);
                if(duration.count()>=(time*1.0*60-1)*1000)
                    return;
                if (prob(gen) >= threshold)
                {
                    next_state_random(used_locations, contri, unused_locations);
                    threshold = (double)(it - iterations) / (double)(it * 1.0);
                }
                else
                {
                    int init_bc = curr_best_cost;
                    next_state_greedy(used_locations, contri, unused_locations);
                    if (curr_best_cost >= init_bc)
                        rand_flag = true;
                    if (curr_best_cost < init_bc)
                        rand_flag = false;
                    threshold = (double)(it - iterations) / (double)(it * 1.0);
                }
            }

            if (curr_best_cost < best_cost)
            {
                best_cost = curr_best_cost;
                for (int i = 0; i < z; i++)
                    best_mp[i] = curr_best_mp[i];
            }
            globalstop = high_resolution_clock::now();
            globalduration = duration_cast<milliseconds>(globalstop - start);
        }

        return;
    }


    int * SportsLayout::find_best_mapping()
    {
        int *best_mp = new int[z];
        int best_cost = INT_MAX;
        // this->simulated_annealing(best_mp, best_cost);
        this->greedy_with_restarts(best_mp, best_cost);
        return best_mp;
    }
