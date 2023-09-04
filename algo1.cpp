#include "SL.h"

int *SportsLayout::find_best_mapping()
{
    int *best_mp = new int[z];
    int best_cost = INT_MAX;
    // this->simulated_annealing(best_mp, best_cost);
    this->greedy_with_restarts(best_mp, best_cost);
    return best_mp;
}

void SportsLayout::greedy_with_restarts(int *best_mp, int &best_cost)
{
  auto start = high_resolution_clock::now();
  double time_var = (time * 1.0 * 60 - 1) * 1000;
  int *curr_mp = new int[z];
  long long curr_cost, curr_best_cost = INT_MAX;
  int *curr_best_mp = new int[z];

  auto next_state_greedy = [&](vector<bool> &used_locations, set<pair<long long, int>> &contri)
  {
    pair<long long, int> pr = *(--contri.end());
    int i = pr.second;
    long long initial_contribution = pr.first;
    int curr_location = curr_mp[i];
    int best_location = curr_location;
    long long best_location_cost = curr_cost;

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
      if (duration.count() >= time_var)
        goto exit_label;
      // return;

      next_state_greedy(used_locations, contri);
    }

  exit_label:
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
