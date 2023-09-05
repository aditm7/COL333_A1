all:
	g++ -o ./bin/main%.out main.cpp SL.cpp algo%.cpp -std=c++17

# 1. greedy_hill_climbing -> make run1
# 2. simulated_annealing -> make run2
# 3. totally random -> make run3

run%:
	g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17
	./bin/main$*.out ./io_files/in.txt ./io_files/out$*.txt
gen:
	g++ ./generator/gen.cpp -o ./bin/gen.out -std=c++17
	./bin/gen.out