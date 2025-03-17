#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <queue>
// #include <stack>
#include <vector>
#include <cmath>
#include "Point.h"

static const int dr[] = {-1, 1, 0, 0};
static const int dc[] = {0, 0, 1, -1};

class Map {
    // Member Variables
    struct Node {
        char type;

        Node(char c);
        Node();
        // ~Node();
    };

    struct ComparePoint {
        Point destination;
    
        ComparePoint(const Point &fin) : destination(fin) {}

        float distance(const Point &current) const { 
            float distance;
            float yDiff = pow(destination.lat-current.lat, 2);
            float xDiff = pow(destination.lng-current.lng, 2);
            distance = sqrt(yDiff + xDiff);
            return distance;
        }

        bool operator()(const Point &a, const Point &b) const {
            return distance(a) > distance(b);
        }        
    };

public:

    Map(std::istream& stream);
    ~Map();

    Node** arr;
    bool** visited;
    int columns, rows;

    Point fin;

    void neighbors(Point current, std::priority_queue<Point, std::vector<Point>, ComparePoint> &pq);

    bool CheckStartPoint(Point start);
    bool CheckEndPoint(Point end);

    std::string route(Point src, Point dst);
};

#endif
