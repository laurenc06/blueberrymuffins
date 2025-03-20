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

static const int dr[] = {-1, 1, 0, 0};
static const int dc[] = {0, 0, 1, -1};

class Map {
    // Member Variables

    // to figure out if there is a bomb or not
    struct SearchState {
        int lat;
        int lng;
        int bombs;
        std::string route;

        SearchState(int y, int x, int b, const std::string &r) {
            lat = y;
            lng = x;
            bombs = b;
            route = r;
        }

    };

    struct CompareStates {
        SearchState destination;
    
        CompareStates(const SearchState &fin) : destination(fin) {};

        float distance(const SearchState &current) const { 
            float distance;
            float yDiff = pow(destination.lat-current.lat, 2);
            float xDiff = pow(destination.lng-current.lng, 2);
            distance = sqrt(yDiff + xDiff);
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
    // bool*** visitedStates;
    int columns, rows;
    //int maxBombCount(0), maxBouldersCount(0);

    UnionFind uf;

    Point fin;

    void neighbors(const SearchState &current, const Point &dst, std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> &pq);

    bool CheckStartPoint(Point start);
    bool CheckEndPoint(Point end);

    std::string route(Point src, Point dst);
};

#endif
