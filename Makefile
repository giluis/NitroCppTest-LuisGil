main: main.cpp
	gcc -std=c++17 -o main main.cpp -lboost_json -lstdc++
run: main
	./main