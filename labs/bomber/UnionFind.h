#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
#include <map>
#include "Node.h"
#include "Point.h"

using namespace std;

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
        bool shouldBomb(Node current, Node neighbor, Node end);
        bool isWalkable(Node cell);
        void numBombs();
};

#endif
