all: main.cpp temp_adit.cpp
	  g++ -o ./bin/main.out main.cpp temp_adit.cpp -std=c++17

run:
	g++ -o ./bin/main.out main.cpp temp_adit.cpp -std=c++17
	./bin/main.out ./io_files/in.txt ./io_files/out.txt

gen:
	g++ ./generator/gen.cpp -o ./bin/gen.out -std=c++17
	./bin/gen.out