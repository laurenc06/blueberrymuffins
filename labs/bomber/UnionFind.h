#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <vector>
#include <map>
#include "Node.h"
#include "Point.h"

using namespace std;

class UnionFind {
    vector<int> parents;
    vector<bool> visited;
    vector<int> rank;
    map<int,Node> points;
    int cols; // # of columns
    int rows; // # of rows

    public: 
        UnionFind();
        void setCols(int c);
        void insert(int x, Node point);
        int find(int x);
        int findIndex(Node current);
        void unite(int a, int b);
        //void connectPoints(int index);
        void connectAll();
        bool shouldBomb(Node current, Node neighbor, Node end);
};

#endif
