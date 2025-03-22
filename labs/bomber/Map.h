#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <cmath>
#include "Point.h"
#include "UnionFind.h"
#include "Node.h"
#include <set>
#include <cstdlib>
#include <unordered_map>

static const int dr[] = {-1, 1, 0, 0};
static const int dc[] = {0, 0, 1, -1};
static const char directions[] = {'n', 's', 'e', 'w'};

class Map {
    // Member Variables

    // to figure out if there is a bomb or not
    struct SearchState {
        int lat;
        int lng;
        int bombs;
        std::string route;
        std::map<int,Point> pickedUpBombs;

        SearchState(int y, int x, int b, const std::string& r, std::map<int,Point> bombsPU) {
            lat = y;
            lng = x;
            bombs = b;
            route = r;
            pickedUpBombs = bombsPU;
        }

    };

    struct CompareStates {
        SearchState destination;
    
        CompareStates(const SearchState &fin) : destination(fin) {};

        int distance(const SearchState &current) const { 
            int distance;
            int yDiff = std::abs(destination.lat-current.lat);
            int xDiff = std::abs(destination.lng-current.lng);
            distance = yDiff+xDiff;
            return distance;
        }

        bool operator()(const SearchState &a, const SearchState &b) const {
            return distance(a) > distance(b);
        }        
    };

public:

    Map(std::istream& stream);
    ~Map();

    Node** grid;
    int columns, rows;
    int maxBombCount=0;
    int maxBouldersCount=0;
    map<int,Node> allBombs;

    UnionFind uf;

    // Point fin;

    void neighbors(SearchState &current, const Point &dst, std::queue<SearchState> &stateQueue, UnionFind& thisUF, std::set<std::tuple<int,int,int>>& visited);
    bool CheckStartPoint(Point start);
    bool CheckEndPoint(Point end);

    std::string route(Point src, Point dst);

    private:
        bool isWalkable(Node cell);
        void bombingSim(Node boulder, UnionFind& thisUF); 
    };

#endif
