#include "Map.h"
#include "Errors.h"
#include <unordered_set>

// Member Functions

Map::~Map() {
    if (grid) {
        for (int i = 0; i < rows; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }

    columns = 0;
    rows = 0;
}

Map::Map(std::istream& stream) {
    std::vector<std::string> allData;
    std::string line;
    while (getline(stream, line)) {
        if (!line.empty()) {
            allData.push_back(line);
        }
    }

    rows = static_cast<int>(allData.size());
    columns = static_cast<int>(allData[0].length());

    uf = UnionFind(rows, columns);
    grid = new Node*[rows]();

    for (int i = 0; i < rows; i++) {
        grid[i] = new Node[columns];
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            //update maxBombCount
            if(allData[y][x] == '*')
                maxBombCount++;
            //update maxBouldersCount
            else if(allData[y][x] == '#')
                maxBouldersCount++;
            grid[y][x] = Node(allData[y][x], y, x);
        }
    }

    //connect the connected nodes in uf
    uf.connectAll((const Node**)grid);

    //updates num of bombs per region
    uf.assignBombs((const Node**)grid);
}

std::string Map::route(Point src, Point dst) {
    if(!CheckStartPoint(src)) {
        throw PointError(src);
    }

    else if(!CheckEndPoint(dst)) {
        throw PointError(dst);
    }

    // fin = dst; // do we need?
    int initialBombCount = 0;
    
    std::set<std::tuple<int, int, int>> visited;
    std::queue<SearchState> stateQueue;

    UnionFind thisUF = uf;

    if (grid[src.lat][src.lng].type == '*') {
        initialBombCount = 1;
    }

    SearchState initialState(src.lat, src.lng, initialBombCount, "");
    stateQueue.push(initialState);
    
    visited.insert(std::make_tuple(src.lat, src.lng, initialBombCount));

    while (!stateQueue.empty()) {
        SearchState current = stateQueue.front();
        stateQueue.pop();

        if (current.lat == dst.lat && current.lng == dst.lng) {
            return current.route;
        }
        neighbors(current, dst, stateQueue, thisUF, visited);
    }
    throw RouteError(src, dst);
}

void Map::neighbors(const SearchState &current, const Point &dst, std::queue<SearchState> &stateQueue, UnionFind& thisUF, std::set<std::tuple<int,int,int>>& visited) {
    for (int d = 0; d < 4; d++) {
        int neighborY = current.lat + dr[d];
        int neighborX = current.lng + dc[d];

        if (neighborY < 0 || neighborX < 0 || neighborX >= columns || neighborY >= rows) {
            continue;
        }

        char cellType = grid[neighborY][neighborX].type;
        int newBombCount = current.bombs;
        bool canVisit = false;

        if (cellType == '.' || cellType == '*') {
            canVisit = true;
            if (cellType == '*' && !visited.count(std::make_tuple(neighborY, neighborX, newBombCount))) {
                newBombCount++;
                //std::min(newBombCount + 1, maxBouldersCount);
            }
        }
        else if (cellType == '#') {
            if (newBombCount > 0) {
                int neighborDist = abs(dst.lat-neighborY) + abs(dst.lng-neighborX);
                int currentDist = abs(dst.lat-current.lat) + abs(dst.lng-current.lng);
                if(neighborY == dst.lat && neighborX == dst.lng) //if neighbor is the destination
                {
                    canVisit = true;
                    newBombCount--;
                }
                else if (thisUF.shouldBomb((const Node**)grid, grid[current.lat][current.lng], grid[neighborY][neighborX], grid[dst.lat][dst.lng]) || neighborDist<currentDist) { 
                    canVisit = true;
                    //std::cout << "should bomb " << neighborY << " " << neighborX << std::endl;
                    // bombingSim(grid[neighborY][neighborX], thisUF);
                    newBombCount--;   
                }
            }
        }

        if (canVisit) {
            auto neighborState = std::make_tuple(neighborY, neighborX, newBombCount);
            if (!visited.count(neighborState)) {
                visited.insert(neighborState);
                SearchState next(neighborY, neighborX, newBombCount, current.route+directions[d]);
                stateQueue.push(next);
            }
        }
    }
}

bool Map::isWalkable(Node cell) {
    if (cell.type == '*' || cell.type == '.') {
        return true;
    }
    return false;
}

// void Map::bombingSim(Node boulder, UnionFind& thisUF) {
//     int bRow = boulder.y;
//     int bCol = boulder.x;
//     int boulderIdx = thisUF.getIndex(boulder);

//     if (bCol + 1 < columns && isWalkable(grid[bRow][bCol+1])) {
//         thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow][bCol+1]));
//     }
//     if (bCol - 1 >= 0 && isWalkable(grid[bRow][bCol-1])) {
//         thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow][bCol-1]));
//     }
//     if (bRow + 1 < rows && isWalkable(grid[bRow+1][bCol])) {
//         thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow+1][bCol]));
//     }
//     if (bRow - 1 >= 0 && isWalkable(grid[bRow-1][bCol])) {
//         thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow-1][bCol]));
//     }
// }

bool Map::CheckStartPoint(Point start) {
    if (start.lat < 0 || start.lng < 0 || start.lat > rows - 1 || start.lng > columns - 1) {
        return false;
    }

    char t = grid[start.lat][start.lng].type;
    if (t != '.' && t != '*') {
        return false;
    }
    return true;
}

bool Map::CheckEndPoint(Point end) {
    if (end.lat < 0 || end.lng < 0 || end.lat > rows - 1 || end.lng > columns - 1) {
        return false;
    }
    return true;
}