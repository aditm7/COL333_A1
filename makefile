all: main.cpp SL.cpp
	  g++ -o ./bin/main main.cpp SL.cpp -std=c++17

run:
	g++ -o ./bin/main main.cpp SL.cpp -std=c++17
	./bin/main ./io/in.txt ./io/out.txt

gen:
	g++ ./generator/gen.cpp -o ./bin/gen.cpp -std=c++17
	./bin/gen.cpp 