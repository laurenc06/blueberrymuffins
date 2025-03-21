#include "UnionFind.h"

UnionFind::UnionFind() {}

UnionFind::UnionFind(int r, int c) {
    rows = r;
    cols = c;
    parents.resize(rows*cols);
    rank.resize(rows*cols, 0);

    for (int i =0; i < rows*cols; i++) {
        parents[i] = i;
    }
}

int UnionFind::find(int x)
{
    if (parents[x] != x)
    {
        parents[x] = find(parents[x]);
    }
    return parents[x];
}

void UnionFind::unite(int a, int b)
{
    int aRoot = find(a);
    int bRoot = find(b);
    if(aRoot!=bRoot)
    {
        if(rank[aRoot] > rank[bRoot])
            parents[bRoot] = aRoot;
        else if(rank[aRoot] < rank[bRoot])
            parents[aRoot] = bRoot;
        else {
            parents[bRoot] = aRoot;
            rank[aRoot]++;
        }
    }
}

bool UnionFind::isWalkable(Node cell) {
    return (cell.type == '.' || cell.type == '*');
}

void UnionFind::connectAll(const Node** grid)
{
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            
            Node cell = grid[i][j];
            if(isWalkable(cell))
            {
                int index = getIndex(grid[i][j]);

                //if in bounds and valid check east
                if (j + 1 < cols && isWalkable(grid[i][j+1])) 
                {
                    unite(index, getIndex(grid[i][j+1]));
                }
                //if in bounds and valid check south
                if(i + 1 < rows && isWalkable(grid[i+1][j]))
                {
                    unite(index, getIndex(grid[i+1][j]));
                }
            }
        }
    }

}

//gets the index of the node in map points
int UnionFind::getIndex(Node current)
{
    return current.y*cols + current.x;
}

//gets the index of the node in map points
int UnionFind::getIndex(int r, int c) {
    return r*cols + c;
}

// function to check if we should bomb given current cell is at (y, x) and the neighbor is at (ny, nx)
// check if bombing it would connect you to destination
bool UnionFind::shouldBomb(const Node** grid, Node current, Node boulder, Node end) {
    int currentIndex = getIndex(current);
    int endIndex = getIndex(end);
    
    if(find(currentIndex) == find(endIndex)) // first check if theyre in the same region. if so, no need to bomb
        return false;

    int bRow = boulder.y;
    int bCol = boulder.x;

    // since ny, nx will be the boulder's coords, we check if the adjacent traversable cell to it is in same set as destination
    if(bCol+1 < cols) // east neighbor
    {
        int index = getIndex(bRow, bCol + 1);
        if(find(index) == find(endIndex))
            return true;
        //check if bombing leads to region w 1+ bombs
        if(numBombs[find(index)] >= 1)
            return true;
    }
    if((bCol)-1 >= 0) // west neighbor
    {
        int index = getIndex(bRow, bCol - 1);
        if(find(index) == find(endIndex))
            return true;
        //check if bombing leads to region w 1+ bombs
        if(numBombs[find(index)] >= 1)
            return true;
    }
    if(bRow - 1 >= 0) // north neighbor
    {
        int index = getIndex(bRow -1, bCol);
        if(find(index) == find(endIndex))
            return true;
        //check if bombing leads to region w 1+ bombs
        if(numBombs[find(index)] >= 1)
            return true;
    }
    if(bRow +1 < rows) // south neighbor
    {
        int index = getIndex(bRow +1, bCol);
        if(find(index) == find(endIndex))
            return true;
        //check if bombing leads to region w 1+ bombs
        if(numBombs[find(index)] >= 1)
            return true;
    }
    return false;
}

void UnionFind::assignBombs(const Node** grid)
{
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
            Node current = grid[i][j];
            if(isWalkable(current))
            {
                if(current.type == '*')
                {
                    numBombs[find(getIndex(current))]++;
                    //std::cout << "current num bombs at " << find(getIndex(current)) << " is " << numBombs[getIndex(current)] << std::endl;
                }
            }
        }

    }
    /*
    std::cout << "numBombs map:" << std::endl;
    for(const auto& pair: numBombs)
    {
        std::cout << pair.first << " " << pair.second << std::endl;
    }
    */
}