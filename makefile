# 1. greedy_hill_climbing -> make run1
# 2. simulated_annealing -> make run2
# 3. totally random -> make run3
%:
	@g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17 -O3
run%:
	@g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17 -O3
	@./bin/main$*.out ./io_files/in.txt ./out.txt
gen:
	@g++ ./generator/gen.cpp -o ./bin/gen.out -std=c++17
	@./bin/gen.out

test%:
	@g++ -o ./bin/main$*.out main.cpp SL.cpp algo$*.cpp -std=c++17 -O3
	@./automate.sh

zip:
	zip 2020EE10458_2020EE10490.zip SL.h SL.cpp main.cpp algo.cpp compile.sh run.sh writeup.txt