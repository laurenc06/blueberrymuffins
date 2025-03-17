bomber: Point.h Point.cpp Map.h Map.cpp main.cpp Errors.h
	g++ -c Point.cpp -o Point.o -std=c++17 -Wall -Werror
	g++ -c Map.cpp -o Map.o -std=c++17 -Wall -Werror
	g++ Map.o Point.o main.cpp -o bomber -std=c++17 -Wall -Werror
clean: 
	rm -f *.o bomber
