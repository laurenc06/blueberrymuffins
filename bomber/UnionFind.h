#include <vector>
#include <map>
#include "Map.h"
#include "Point.h"

using namespace std;

class UnionFind {
    vector<int> parents;
    vector<bool> visited;
    map<Map::Node,int> points;
    int cols; //# of columns

    public: 
        UnionFind();
        void setCols(int c);
        void insert(Map::Node point, int x);
        int find(int x);
        void unite(int a, int b);
        void connectPoints(Map::Node point);
        void connectAll();

        bool shouldBomb(int y, int x, int ny, int nx, Point end);
};
