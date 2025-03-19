#include "UnionFind.h"

UnionFind::UnionFind()
{
}

void UnionFind::setCols(int c)
{
    cols = c;
}

void UnionFind::insert(Map::Node point, int x)
{
    points[point] = x;
    parents.push_back(x);
    visited.push_back(false);
}

int UnionFind::find(int x)
{
    if(parents[x] != x)
        parents[x] = find(parents[x]);
    return parents[x];
}

void UnionFind::unite(int a, int b)
{
    int aRoot = find(a);
    int bRoot = find(b);
    if(aRoot!=bRoot)
        parents[aRoot] = bRoot; //doesn't matter which one becomes root (?)
    return;
}

void UnionFind::connectPoints(Map::Node point)
{
    while(point.y >= 0 && point.x >= 0 && point.type != '~' && point.type != '#')
    {
        unite(points[point], (points[point])+1); //unite w east neighbor
        unite(points[point], (points[point])-1); //unite w west neighbor
        unite(points[point], (points[point])-cols); //unite w north neighbor
        unite(points[point], (points[point])+cols); //unite w south neighbor
    }
}

void UnionFind::connectAll()
{
    if (points.size()==0)
        return;
}

// function to check if we should bomb given current cell is at (y, x) and the neighbor is at (ny, nx)
// check if bombing it would connect you to destination
bool UnionFind::shouldBomb(int y, int x, int ny, int nx, Point end) {
    // first check if theyre in the same region. if so, no need to bomb
    // find(current) == find(dest) { return false; }

    // since ny, nx will be the boulder's coords, we check if the adjacent traversable cell to it is in same set as destination
    // if (find(adjacent) == find(dest)) { return true;}

    return false;
}

//add new var to count ins
//add after line 45 in Map.cpp
//(UnionFindObjName).insert(ins,Node(type,y,x))
//increment ins after every insert
