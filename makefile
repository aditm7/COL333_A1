all: main.cpp SL.cpp
	  g++ -o ./bin/main.out main.cpp SL.cpp -std=c++17

run:
	g++ -o ./bin/main.out main.cpp SL.cpp -std=c++17
	./bin/main.out ./io/in.txt ./io/out.txt

gen:
	g++ ./generator/gen.cpp -o ./bin/gen.out -std=c++17
	./bin/gen.out