#include "UnionFind.h"

UnionFind::UnionFind()
{
}

void UnionFind::insert(int x, Map::Node point)
{
    points[x] = point;
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

void UnionFind::connectPoints()
{
    if(points.size()==0)
        return;
    Map::Node current = points[0];
    while(current.y >= 0 && current.x >= 0 && current.type != '~' && current.type != '#')
    {

    }
}

//add new var to count ins
//add after line 45 in Map.cpp
//(UnionFindObjName).insert(ins,Node(type,y,x))
//increment ins after every insert
