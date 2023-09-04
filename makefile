all:
	g++ -o ./bin/main1.out main.cpp SL.cpp algo1.cpp -std=c++17
	g++ -o ./bin/main2.out main.cpp SL.cpp algo2.cpp -std=c++17
run1: # greedy_hill_climbing
	g++ -o ./bin/main1.out main.cpp SL.cpp algo1.cpp -std=c++17
	./bin/main1.out ./io_files/in.txt ./io_files/out.txt
run2: # simulated_annealing
	g++ -o ./bin/main2.out main.cpp SL.cpp algo2.cpp -std=c++17
	./bin/main2.out ./io_files/in.txt ./io_files/out.txt
gen:
	g++ ./generator/gen.cpp -o ./bin/gen.out -std=c++17
	./bin/gen.out