#include "UnionFind.h"

UnionFind::UnionFind()
{
}

void UnionFind::setCols(int c)
{
    cols = c;
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

void UnionFind::connectPoints(int index)
{
    while((points[index]).y >= 0 && (points[index]).x >= 0 && (points[index]).type != '~' && (points[index]).type != '#')
    {
        //call connectPoints for all of the neighbors
        connectPoints(index+1);
        connectPoints(index-1);
        connectPoints(index-cols);
        connectPoints(index+cols);
        unite(index, index+1); //unite w east neighbor
        unite(index, index-1); //unite w west neighbor
        unite(index, index-cols); //unite w north neighbor
        unite(index, index+cols); //unite w south neighbor
    }
}

void UnionFind::connectAll()
{
    if(points.size()==0)
        return;
    for(int i=0; i<parents.size(); i++) //iterate through parents (0 to 1-#ofpointsingrid)
    {
        while(!visited[i] && (points[i]).type != '~' && (points[i]).type != '#') //while the current point is NOT visited, it's not a ~ or #
            connectPoints(i); //call connectPoints on that point
    }
}

//IN MAP.CPP
//must setCols somewhere
//add new var to plug in as a param for insert function
//(UnionFindObjName).insert(ins,Node(type,y,x))
//increment ins after every insert
