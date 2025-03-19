#include "UnionFind.h"

UnionFind::UnionFind()
{
}

void UnionFind::setCols(int c)
{
    cols = c;
    rows = (parents.size())/cols;
}

void UnionFind::insert(int x, Node point)
{
    points[x] = point;
    parents.push_back(x);
    visited.push_back(false);
}

int UnionFind::find(int x)
{
    if(parents[x] && parents[x] != x)
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
    if (points.size()==0)
        return;
    for(int i=0; i< static_cast<int>(parents.size()); i++) //iterate through parents (0 to 1-#ofpointsingrid)
    {
        while(!visited[i] && (points[i]).type != '~' && (points[i]).type != '#') //while the current point is NOT visited, it's not a ~ or #
            connectPoints(i); //call connectPoints on that point
    }
}

//finds the index of the node in map points
int UnionFind::findIndex(Node current)
{
    for (const auto& point: points)
    {
        Node temp = Node(point.second);
        if(current.type == temp.type && current.y == temp.y && current.x == temp.x)
        {
            int key = point.first;
            return key;
        }
    }  
    return -1;
}

// function to check if we should bomb given current cell is at (y, x) and the neighbor is at (ny, nx)
// check if bombing it would connect you to destination
bool UnionFind::shouldBomb(Node current, Node neighbor, Node end) {
    int currentIndex = findIndex(current);
    int neighborIndex = findIndex(neighbor);
    int endIndex = findIndex(end);
    Node adjacent;
    if(find(currentIndex) != find(neighborIndex)) // first check if theyre in the same region. if so, no need to bomb
        return false;

    // since ny, nx will be the boulder's coords, we check if the adjacent traversable cell to it is in same set as destination
    if((currentIndex)+1 < cols) // east neighbor
    {
        if(find(currentIndex+1) == find(endIndex))
            return true;
    }
    if((currentIndex)-1 >= 0) // west neighbor
    {
        if(find(currentIndex-1) == find(endIndex))
            return true;
    }
    if((currentIndex)-cols >= 0) // north neighbor
    {
        if(find(currentIndex-cols) == find(endIndex))
            return true;
    }
    if((currentIndex)+cols < rows) // south neighbor
    {
        if(find(currentIndex+cols) == find(endIndex))
            return true;
    }
    return false;
}

//IN MAP.CPP
//must setCols somewhere
//add new var to plug in 
//increment ins after every insert
