#include "SL.h"

int *SportsLayout::find_best_mapping()
{
    int *best_mp = new int[z];
    int best_cost = INT_MAX;
    this->simulated_annealing(best_mp, best_cost);
    // this->greedy_with_restarts(best_mp, best_cost);
    return best_mp;
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
  double time_var = (time * 1.0 * 60 - 1) * 1000;
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
      if (duration.count() >= (time * 1.0 * 60 - 1) * 1000)
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
