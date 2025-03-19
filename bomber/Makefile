bomber: Point.h Point.cpp Map.h Map.cpp main.cpp Errors.h UnionFind.cpp UnionFind.h Node.h
	g++ -c Point.cpp -o Point.o -std=c++17 -Wall -Werror
	g++ -c Map.cpp -o Map.o -std=c++17 -Wall -Werror
	g++ -c UnionFind.cpp -o UnionFind.o -std=c++17 -Wall -Werror
	g++ Map.o Point.o UnionFind.o Node.h main.cpp -o bomber -std=c++17 -Wall -Werror
clean: 
	rm -f *.o bomber
