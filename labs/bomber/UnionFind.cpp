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
// rewritten to get rid of the recursion problem
bool UnionFind::shouldBomb(const Node** grid, Node current, Node boulder, Node end, int bombs) {
    if (bombs <= 0) return false;

    int endIndex = getIndex(end);
    // only allow paths that use up to 'bombs' bombs

    std::queue<std::pair<Node, int>> q;
    std::set<int> visited;
    static const int dr[] = {-1, 1, 0, 0};
    static const int dc[] = {0, 0, 1, -1};

    q.push({boulder, bombs}); // is it -1 or not -1
    visited.insert(getIndex(boulder));
    
    while (!q.empty()) {
        auto [node, remainingBombs] = q.front();
        q.pop();

        for (int i = 0; i < 4; i++) {
            int ny = node.y + dr[i];
            int nx = node.x + dc[i];

            if (ny < 0 || ny >= rows || nx < 0 || nx >= cols) continue;

            Node neighbor = grid[ny][nx];

            if(visited.count(getIndex(neighbor))) continue;
            visited.insert(getIndex(neighbor));

            if (isWalkable(neighbor)) {
                if (find(getIndex(neighbor)) == find(endIndex)) return true;
                if (numBombs[find(getIndex(neighbor))] > 0) return true;
            }
            else if (neighbor.type == '#' && remainingBombs > 1) {
                q.push({neighbor,remainingBombs -1});
            }
        }
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

