#include <vector>
#include <map>
#include "Node.h"
#include "Point.h"

using namespace std;

class UnionFind {
    vector<int> parents;
    vector<bool> visited;
    map<int,Map::Node> points;
    int cols; // # of columns
    int rows; // # of rows

    public: 
        UnionFind();
        void setCols(int c);
        void insert(int x, Map::Node point);
        int find(int x);
        int findIndex(Map::Node current);
        void unite(int a, int b);
        void connectPoints(int index);
        void connectAll();
        bool shouldBomb(Map::Node current, Map::Node neighbor, Map::Node end);
};
