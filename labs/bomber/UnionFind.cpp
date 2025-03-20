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
    rank.push_back(x);
    visited.push_back(false);
}

int UnionFind::find(int x)
{
    int numElements = rows*cols;
    if (x>=0 && x<numElements)
    {
        if(parents[x] && parents[x] != x)
            parents[x] = find(parents[x]);
    }
    return x;
}

void UnionFind::unite(int a, int b)
{
    int aRoot = find(a);
    int bRoot = find(b);
    if(aRoot!=bRoot)
    {
        if(rank[aRoot] > rank[bRoot])
            parents[aRoot] = bRoot;
        else if(rank[aRoot] < rank[bRoot])
            parents[bRoot] = aRoot;
        else {
            parents[bRoot] = aRoot;
            rank[aRoot]++;
        }
    }
}

/*
void UnionFind::connectPoints(int index)
{
    if(index >= 0 && index < (cols*rows)) //if index is in bounds
    {
        if((points[index]).type != '~' && (points[index]).type != '#') //(points[index]).y >= 0 && (points[index]).x >= 0)
        {
            visited[index] = true; //set visited of current index to true
            //call connectPoints for all of the neighbors
            if(index%cols != (cols-1)) //if not at end of row
            {
                connectPoints(index+1);
                unite(index, index+1); //unite w east neighbor
            }
            if(index%cols != 0) //if not at start of row
            {
                connectPoints(index-1);
                unite(index, index-1); //unite w west neighbor
            }
            connectPoints(index-cols);
            unite(index, index-cols); //unite w north neighbor
            connectPoints(index+cols);                
            unite(index, index+cols); //unite w south neighbor
        }
    }
}
*/

void UnionFind::connectAll()
{
    /*
    if (points.size()==0)
        return;
    for(int i=0; i< static_cast<int>(parents.size()); i++) //iterate through parents (0 to 1-#ofpointsingrid)
    {
        while(!visited[i] && (points[i]).type != '~' && (points[i]).type != '#') //while the current point is NOT visited, it's not a ~ or #
            connectPoints(i); //call connectPoints on that point
    }
    */

    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            int current = (i*cols) + j;
            if(!visited[current])
            {
                visited[current] = true;
                //if in bounds and valid check east
                if((j+1)<cols && j%cols != (cols-1) && points[j+1].type != '~' && points[j+1].type != '#')
                {
                    unite(current, current+1);
                    visited[current+1] = true;
                }
                //if in bounds and valid check west
                if((j-1)>=0 && j%cols != 0 && points[j-1].type != '~' && points[j-1].type != '#')
                {
                    unite(current, current-1);
                    visited[current-1] = true;
                }
                //if in bounds and valid check north
                if((i-1)>=0 && points[current-cols].type != '~' && points[current-cols].type != '#')
                {
                    unite(current, current-cols);
                    visited[current-cols] = true;
                }
                //if in bounds and valid check south
                if((i+1)<rows && points[current+cols].type != '~' && points[current+cols].type != '#')
                {
                    unite(current, current+cols);
                    visited[current+cols] = true;
                }
            }
        }
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
    if(find(currentIndex) == find(endIndex)) // first check if theyre in the same region. if so, no need to bomb
        return false;

    // since ny, nx will be the boulder's coords, we check if the adjacent traversable cell to it is in same set as destination
    if((neighborIndex)+1 < cols) // east neighbor
    {
        if(find(neighborIndex+1) == find(endIndex))
            return true;
    }
    if((neighborIndex)-1 >= 0) // west neighbor
    {
        if(find(neighborIndex-1) == find(endIndex))
            return true;
    }
    if((neighborIndex)-cols >= 0) // north neighbor
    {
        if(find(neighborIndex-cols) == find(endIndex))
            return true;
    }
    if((neighborIndex)+cols < rows) // south neighbor
    {
        if(find(neighborIndex+cols) == find(endIndex))
            return true;
    }
    return false;
}

//IN MAP.CPP
//add new var to plug in 
//increment ins after every insert
