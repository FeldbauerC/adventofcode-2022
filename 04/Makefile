main_1: main_part1.cpp
	g++ -Wall -Werror main_part1.cpp -o task1

main_2: main_part2.cpp
	g++ -Wall -Werror main_part2.cpp -o task2

run_1: main_1
	./task1 input

run_2: main_2
	./task2 input

all: main_1 main_2

clean:
	@rm -rf task1
	@rm -rf task2