#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
mt19937 gen(steady_clock::now().time_since_epoch().count());

atomic<bool> exit_flag = false;

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


    int SportsLayout::cost_fn(int* mp){
        int cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=i+1;j<z;j++) 
           {
                cost+=(N[i][j]+N[j][i])*T[mp[i]-1][mp[j]-1];
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
        // uniform_int_distribution<long long> rnd(0,l-1);
        // for(int i=0;i<l;i++) swap(locations[i],locations[rnd(gen)]);
        int* init_mp = new int[z];
        for(int i=0;i<z;i++) init_mp[i]=locations[i];
        return init_mp;
    }

    void SportsLayout::compute_allocation(int* mp)
    {
        for(int i=0;i<z;i++)
        mapping[i]=mp[i];
    }
    
    int SportsLayout::find_contribution(int* mp,int idx){
        int ans=0;
        for(int i=0;i<z;i++){
            ans+=(N[i][idx]+N[idx][i])*T[mp[i]-1][mp[idx]-1];
        }
        return ans;
    }


    pair<int , int *> SportsLayout:: next_state(vector<bool> & used_locations, int * curr_mp, int & curr_cost, int beam_width, int idx, int contri)
    {
        int * new_mp= new int [z];
        for(int i=0;i<z;i++)
            new_mp[i]=curr_mp[i];
        int new_cost=curr_cost;
        int curr_location = new_mp[idx];
        int best_location = curr_location;
        int best_location_cost = new_cost;
        for(int j=0;j<l;j++)
        {
            if(!used_locations[j+1])
            {
                new_cost -= contri;
                new_mp[idx] = j + 1;
                long long temp_contri = find_contribution(new_mp, idx);
                new_cost += temp_contri;

                if (new_cost < best_location_cost)
                {
                    best_location_cost = new_cost;
                    best_location = j + 1;
                }
                    // rollback the changes
                    new_cost -= temp_contri;
                    new_cost += contri;
                    new_mp[idx] = curr_location;
            }
        }
        new_mp[idx] = best_location;
        new_cost=cost_fn(new_mp);
        return {new_cost,new_mp};
    }


    void SportsLayout::beam_search(int *best_mp, int &best_cost)
    {

        int iterations = 2000;
        int beam_width = 80;
        uniform_real_distribution<double> prob(0.0, 1.0);
        set<pair<int,int*>>bestk;
        for(int i=0;i<beam_width;i++)
        {   
            int *temp = generate_random_mapping();
            bestk.insert({cost_fn(temp),temp});
        }

        vector<pair<int,int *>>kstates;

        while (iterations--)
        {
            if (exit_flag)
                goto return_point_label;
            
            double p=0.;


            kstates.clear();
            for(int i=0;i<beam_width;i++){
                kstates.push_back(*bestk.begin());
                bestk.erase(bestk.begin());
            }
            bestk.clear();
            for(auto it: kstates)
            {
                int cur_state_cost=it.first;
                int * cur_state = it.second;
                if(cur_state_cost<best_cost)
                {
                    best_cost=cur_state_cost;
                    for(int i=0;i<z;i++)
                        best_mp[i]=cur_state[i];
                }

                vector<pair<int, int>>neighbours;
                for(int i=0;i<z;i++)
                {
                    int temp_contri=find_contribution(cur_state,i);
                    neighbours.push_back({temp_contri,i});
                }
                for(auto it2: neighbours)
                {
                    vector<bool>used_locations(l+1,false);
                    for(int i=0;i<z;i++)
                        used_locations[cur_state[i]]=1;
                    if(prob(gen)>p)
                    {
                        pair<int,int*>pr=next_state(used_locations,cur_state,cur_state_cost,beam_width,it2.second,it2.first);
                        bestk.insert(pr);
                    }
                    else
                    {
                        int *temp = generate_random_mapping();
                        bestk.insert({cost_fn(temp),temp});
                    }
                }
            }
        }

    return_point_label:
        cout << iterations << endl;
        return;
    }


    int* SportsLayout::find_best_mapping(){
        vector<int*> arr_mp;
        vector<int> arr_costs;
        
        // greedy hill climbing with restarts
        arr_mp.push_back(new int[z]);
        arr_costs.push_back(INT_MAX);
        thread t1(&SportsLayout::beam_search,this,arr_mp[0],ref(arr_costs[0])); 
        
        // signalling all the processes to exit by returning after the time limit
        chrono::duration<double> sleep_duration(time*60.0-1.0);
        this_thread::sleep_for(sleep_duration);
        exit_flag = true;
        t1.join();

        // now computing the best mapping among all algorithms
        int best_cost = arr_costs[0];
        int* best_mp = arr_mp[0];
        for(int i=1;i<(int)(arr_mp).size();i++){
            if(best_cost<arr_costs[i]){
                best_cost = arr_costs[i];
                best_mp = arr_mp[i];
            }
        }
        return best_mp;
    }