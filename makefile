all: main.cpp SL.cpp
	  g++ -o main main.cpp SL.cpp -std=c++17

run:
	./main in_10z_20l.txt out.txt