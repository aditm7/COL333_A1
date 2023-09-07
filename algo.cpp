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

  auto next_state_greedy = [&](vector<short> &used_locations, vector<long long> &contri,pair<long long,int> &mxc)
  {
    int i = mxc.second; // get the index of the element with the highest contribution
    long long initial_contribution = mxc.first; 
    if(rand_flag){
        // this will assign a random index in case we are stuck in a local minima
        i=rnd_z(gen);
        initial_contribution = contri[i];
    }
    int curr_location = curr_mp[i];
    int best_location = curr_location;
    long long best_location_cost = curr_cost;
    int globalswapindex=-1;
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
      else if(used_locations[j+1]!=i)
      {
        short swapindex=used_locations[j+1]; // get the index of the element at location j+1
        long long swapcontri=find_contribution(curr_mp,swapindex); // get the contribution of the element at location j+1

        // swap the elements at location i and j+1
        curr_mp[i]=j+1;
        curr_mp[swapindex]=curr_location;

        long long newicontri=find_contribution(curr_mp,i); // get the contribution of the element at location i
        long long newswapcontri=find_contribution(curr_mp,swapindex); // get the contribution of the element at location j+1

        curr_cost-=initial_contribution; // subtract the contribution of the element at location i
        curr_cost-=swapcontri; // subtract the contribution of the element at location j+1
        curr_cost+=newicontri; // add the contribution of new element at i 
        curr_cost+=newswapcontri; // add the contribution of new element at swapindex
        
        if(curr_cost<best_location_cost)
        {
          best_location_cost=curr_cost;
          globalswapindex=swapindex;
          chance = true;
        }
        // rollback the changes
        curr_cost-=newicontri;
        curr_cost-=newswapcontri;
        curr_cost+=initial_contribution;
        curr_cost+=swapcontri;
        curr_mp[i]=curr_location;
        curr_mp[swapindex]=j+1;
      }
    }


    if(chance==false)
    {
        // covers the case when the best location is found by choosing unused location
        mxc = {0,-1};
        curr_mp[i] = best_location;
        used_locations[curr_location] = -1;
        used_locations[best_location] = i;
        for (int idx = 0; idx < z; idx++){
          if(idx!=i) {
            contri[idx]+=(((long long)N[i][idx] + (long long)N[idx][i]) * (long long)(T[curr_mp[idx]-1][best_location- 1] - T[curr_mp[idx]-1][curr_location - 1]));
            if(mxc.second<contri[idx]) mxc = {contri[idx],idx};
          }
        }
        
        long long best_loc_contri = find_contribution(curr_mp, i);
        contri[i] = best_loc_contri;
        if(mxc.first<contri[i]) mxc = {contri[i],i};
        curr_cost+=best_loc_contri;
        curr_cost-=initial_contribution;

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
        used_locations[curr_mp[i]]=globalswapindex;
        used_locations[curr_mp[globalswapindex]]=i;
        mxc = {0,-1};
        swap(curr_mp[i],curr_mp[globalswapindex]);
        for(int idx=0;idx<z;idx++){
          if(idx!=i && idx!=globalswapindex){
            contri[idx] += (((long long)N[i][idx] + (long long)N[idx][i]) * (long long)(T[curr_mp[idx]-1][curr_mp[i] - 1] - T[curr_mp[idx]-1][curr_mp[globalswapindex] - 1]));
            contri[idx] += (((long long)N[globalswapindex][idx] + (long long)N[idx][globalswapindex]) * (long long)(T[curr_mp[idx]-1][curr_mp[globalswapindex] - 1] - T[curr_mp[idx]-1][curr_mp[i] - 1]));
            if(mxc.second<contri[idx]) mxc = {contri[idx],idx};
          }
        }

        long long final_contri_i=find_contribution(curr_mp,i);
        long long final_contri_globalswapindex=find_contribution(curr_mp,globalswapindex);
        contri[i] = final_contri_i;
        contri[globalswapindex] = final_contri_globalswapindex;
        if(mxc.first<contri[i]) mxc = {contri[i],i};
        if(mxc.first<contri[globalswapindex]) mxc = {contri[globalswapindex],globalswapindex};
        
        curr_cost-=swapindexcontri;
        curr_cost-=initial_contribution;
        curr_cost+=final_contri_i;
        curr_cost+=final_contri_globalswapindex;

        if (curr_cost < curr_best_cost)
        {
            curr_best_cost = curr_cost;
            for (int j = 0; j < z; j++)
            curr_best_mp[j] = curr_mp[j];
        }
    }
    
  };

  vector<short> used_locations(l + 1, -1);
  vector<long long> contri(z);
  pair<long long,int> mxc = {0,-1};
  short same_counts=0;
  double tuning_parameter = 1.1;
  double firstmapparameter=1.0;
  bool par_flag=false;
  uniform_real_distribution<double> rnd_first(0.0,1.0);
  while (!exit_indicator())
  {
    int * temp;
    if(rnd_first(gen)<=firstmapparameter)
      {
        temp = generate_random_mapping();
        if(par_flag==false)
        {
          firstmapparameter=0.5;
          par_flag=true;
        }
      }
    else
      {
        temp= new int[z];
        for(int ind=0;ind<z;ind++)
        {
          temp[ind]=best_mp[ind];
        }
        int num_shuffle=z/10;
        int num_half_shuffle=num_shuffle/2;
        uniform_int_distribution<int>indone(0,z-1);
        uniform_int_distribution<int>indtwo(0,z-1);
        for(int cn=0;cn<num_half_shuffle;cn++)
        {
          int first_ind=indone(gen);
          int second_ind=indtwo(gen);
          swap(temp[first_ind],temp[second_ind]);
        }
      }
    firstmapparameter-=0.05;
    rand_flag=false;
    mxc = {0,-1};
    same_counts=0;

    for (int i = 0; i < z; i++){
      curr_mp[i] = temp[i];
      curr_best_mp[i] = temp[i];
    }

    curr_cost = cost_fn(curr_mp);
    curr_best_cost = curr_cost;

    used_locations.assign(l + 1, -1);
    for (int i = 0; i < z; i++)
      used_locations[curr_mp[i]] = i;

    
    for (int i = 0; i < z; i++){
      contri[i] = find_contribution(curr_mp, i);
      if(mxc.first<contri[i]) mxc = {contri[i],i};
    }
    
    while (true)
    {
      if (exit_indicator() || (short)(tuning_parameter*this->z)==same_counts)
        goto exit_label;
      long long init_bc = curr_best_cost; 
      next_state_greedy(used_locations,contri,mxc);
      if(curr_best_cost>= init_bc) {rand_flag=true;same_counts+=1;}
      else {rand_flag = false;same_counts=0;}
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
