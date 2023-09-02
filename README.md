# COL333_A1

## Parallelising the AI Algorithms 
 - There is a global exit_flag which should signal the processes to exit.
 - For each algorithm, a thread is created and the variables for it to store it's best cost and mapping are stored in the arrays arr_mp and arr_costs
 - Thread is executed by following format:
  
  ```c++
  std::thread threadname(&ClassName::AI_algo,this,arg1,arg2 ...); // to pass arguments by reference use ref(arg)
  ```
  - Next the main process (from which all threads are called) is then is brought to sync, by joining all the threads to it. This means now the main thread will wait for all the threads called to be exited.
  - Exit flag comes in action here. Using goto keyword, the functions are jumped to their respective return_point_labels.
  - Finally the best mapping is returned.

## To-do
- Change restarts and iterations according to the input
- Beam search and genetic algo try out
- simulated annealing :: change learning rate
- remove extra print statements (like restarts)
- change variable types of cost ??
- generate a more equi-possible random array ?
