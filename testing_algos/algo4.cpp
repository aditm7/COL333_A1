#include "SL.h"

int *SportsLayout::find_best_mapping()
{
    int *best_mp = new int[z];
    long long best_cost = LONG_MAX;
    this->greedy_with_restarts(best_mp, best_cost);
    return best_mp;
}

void SportsLayout::greedy_with_restarts(int *best_mp, long long &best_cost)
{
  int *curr_mp = new int[z];
  long long curr_cost, curr_best_cost = LONG_MAX;
  int *curr_best_mp = new int[z];
  uniform_int_distribution<int> rnd_z(0,z-1);   
  bool rand_flag = false;  // flag to track if the next state is good or not

  auto next_state_greedy = [&](vector<short> &used_locations, set<pair<long long, int>> &contri)
  {
    pair<long long, int> pr = *(--contri.end()); // get the element with the highest contribution
    int i = pr.second; // get the index of the element with the highest contribution
    long long initial_contribution = pr.first; 

    if(rand_flag){
        // this will assign a random index in case we are stuck in a local minima
        i=rnd_z(gen);
        initial_contribution = find_contribution(curr_mp,i);
    }

    int curr_location = curr_mp[i];
    int best_location = curr_location;
    long long best_location_cost = curr_cost;
    int globalswapindex=0;
    bool chance=false; // this variable will track if the best location is found by swapping or by using an unused location
    for (int j = 0; j < l; j++)
    {
      if(j+1==curr_location)
        continue;
      if (used_locations[j + 1]==-1)
      {
        curr_mp[i] = j + 1;
        long long temp_contri = find_contribution(curr_mp, i);

        curr_cost -= initial_contribution;
        curr_cost += temp_contri;

        if (curr_cost < best_location_cost)
        {
          best_location_cost = curr_cost;
          best_location = j + 1;
          chance=false;
        }
        // revert back to the original state
        curr_cost -= temp_contri;
        curr_cost += initial_contribution;
        curr_mp[i] = curr_location;
      }
      else
      {
        short swapindex=used_locations[j+1]; // get the index of the element at location j+1

        long long swapcontri=find_contribution(curr_mp,swapindex); // get the contribution of the element at location j+1

        curr_mp[i]=j+1;
        curr_mp[swapindex]=curr_location;
        // swap the elements at location i and j+1

        long long newicontri=find_contribution(curr_mp,i); // get the contribution of the element at location i
        long long newswapcontri=find_contribution(curr_mp,swapindex); // get the contribution of the element at location j+1

        long long tempocost=curr_cost;

        tempocost-=initial_contribution; // subtract the contribution of the element at location i
        tempocost-=swapcontri; // subtract the contribution of the element at location j+1
        tempocost+=newicontri; // add the contribution of new element at i 
        tempocost+=newswapcontri; // add the contribution of new element at swapindex
        if(tempocost<best_location_cost)
        {
          best_location_cost=tempocost;
          globalswapindex=swapindex;
          chance = true;
        }
        // revert to original state
        tempocost-=newicontri;
        tempocost-=newswapcontri;
        tempocost+=initial_contribution;
        tempocost+=swapcontri;
        curr_mp[i]=curr_location;
        curr_mp[swapindex]=j+1;

      }
    }


    if(chance==0)
    {
        // covers the case when the best location is found by choosing unused location

        curr_mp[i] = best_location;
        used_locations[curr_location] = -1;
        used_locations[best_location] = i;
        contri.erase(--contri.end());
        long long best_loc_contri=find_contribution(curr_mp, i);
        contri.insert({best_loc_contri, i});
        curr_cost = cost_fn(curr_mp);
        
        // curr_cost+=best_loc_contri;
        // curr_cost-=initial_contribution;

        if (curr_cost < curr_best_cost)
        {
            curr_best_cost = curr_cost;
            for (int j = 0; j < z; j++)
            curr_best_mp[j] = curr_mp[j];
        }
    }
    else
    {
        // covers the case when the location is found by swapping within the mapping
        long long swapindexcontri= find_contribution(curr_mp,globalswapindex);
        contri.erase(--contri.end());
        if(contri.find({swapindexcontri,globalswapindex})!=contri.end())
            contri.erase({swapindexcontri,globalswapindex});
        

        used_locations[curr_mp[i]]=globalswapindex;
        used_locations[curr_mp[globalswapindex]]=i;

        // curr_cost+= ((long long)N[i][globalswapindex] + (long long)N[globalswapindex][i]) * (long long)T[curr_mp[i] - 1][curr_mp[globalswapindex] - 1];

        swap(curr_mp[i],curr_mp[globalswapindex]);
        
        long long final_contri_i=find_contribution(curr_mp,i);
        long long final_contri_globalswapindex=find_contribution(curr_mp,globalswapindex);
        contri.insert({final_contri_i,i});
        contri.insert({final_contri_globalswapindex,globalswapindex});

        // curr_cost-=swapindexcontri;
        // curr_cost-=initial_contribution;
        // curr_cost-= ((long long)N[i][globalswapindex] + (long long)N[globalswapindex][i]) * (long long)T[curr_mp[i] - 1][curr_mp[globalswapindex] - 1];
        // curr_cost+=final_contri_i;
        // curr_cost+=final_contri_globalswapindex;

        curr_cost=cost_fn(curr_mp);

        if (curr_cost < curr_best_cost)
        {
            curr_best_cost = curr_cost;
            for (int j = 0; j < z; j++)
            curr_best_mp[j] = curr_mp[j];
        }
    }
    
  };

  vector<short> used_locations(l + 1, -1);

  while (!exit_indicator())
  {
    int *temp = generate_random_mapping();

    rand_flag=false;

    for (int i = 0; i < z; i++)
    {
      curr_mp[i] = temp[i];
      curr_best_mp[i] = temp[i];
    }

    curr_cost = cost_fn(curr_mp);
    curr_best_cost = curr_cost;

    used_locations.assign(l + 1, -1);

    for (int i = 0; i < z; i++)
      used_locations[curr_mp[i]] = i;

    set<pair<long long, int>> contri;

    for (int i = 0; i < z; i++)
      contri.insert({find_contribution(curr_mp, i), i});

    int iterations = this->it;

    while (iterations--)
    {
      if (exit_indicator())
        goto exit_label;
      long long init_bc = curr_best_cost; 
      next_state_greedy(used_locations, contri);
      if(curr_best_cost>= init_bc) rand_flag=true;
      else rand_flag = false;
    }
  exit_label:

    if (curr_best_cost < best_cost)
    {
      best_cost = curr_best_cost;
      for (int i = 0; i < z; i++)
        best_mp[i] = curr_best_mp[i];
    }
  }
  return;
}
