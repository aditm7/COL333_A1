# 1. greedy_hill_climbing -> make run1
# 2. simulated_annealing -> make run2
# 3. totally random -> make run3
%:
	g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17
run%:
	g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17
	./bin/main$*.out ./io_files/in.txt ./io_files/out$*.txt
gen:
	g++ ./generator/gen.cpp -o ./bin/gen.out -std=c++17
	./bin/gen.out

test%:
	g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17
	./automate.sh