#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
#include <map>
#include "Node.h"
#include "Point.h"

using namespace std;

static const int r[] = {-1, 1, 0, 0};
static const int c[] = {0, 0, 1, -1};

class UnionFind {
    vector<int> parents;
    vector<int> rank;
    map<int, int> numBombs;
    int cols; // # of columns
    int rows; // # of rows

    public: 
        UnionFind();
        UnionFind(int r, int c);
        int find(int x);
        int getIndex(Node current);
        int getIndex(int r, int c);
        void unite(int a, int b);
        void connectAll(const Node** grid);
        bool shouldBomb(const Node** grid, Node current, Node boulder, Node end);
        //bool neighborsUF(const Node** grid, Node current, int index);
        bool isWalkable(Node cell);
        void assignBombs(const Node** grid);
};

#endif
