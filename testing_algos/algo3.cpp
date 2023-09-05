#include "SL.h"
//Totally random

int *SportsLayout::find_best_mapping()
{
    int *best_mp = new int[z];
    int best_cost = INT_MAX;
    int max_cost=INT_MIN;
    int* curr_mp;
    int curr_cost;
    while(!exit_indicator()){
      curr_mp = generate_random_mapping();
      curr_cost = cost_fn(curr_mp);
      if(best_cost>curr_cost){
        for(int i=0;i<z;i++) best_mp[i] = curr_mp[i];
      }
      max_cost = max(curr_cost,max_cost);
    }
    cout<<"max cost:"<<" "<<max_cost<<endl;
    return best_mp;
}