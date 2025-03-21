#include "Map.h"
#include "Errors.h"

// Member Functions

const char directions[] = {'n', 's', 'e', 'w'};

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
}

std::string Map::route(Point src, Point dst) {
    if(!CheckStartPoint(src)) {
        throw PointError(src);
    }

    else if(!CheckEndPoint(dst)) {
        throw PointError(dst);
    }

    fin = dst;
    int initialBombCount = 0;
    
    std::set<std::tuple<int, int, int>> visited;
    std::map<std::tuple<int, int, int>, std::pair<std::tuple<int, int, int>, char>> parent;
    std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> stateQueue(CompareStates(SearchState(dst.lat, dst.lng, 0)));

    UnionFind thisUF = uf;

    if (grid[src.lat][src.lng].type == '*') {
        initialBombCount = 1;
    }

    SearchState initialState(src.lat, src.lng, initialBombCount);
    stateQueue.push(initialState);
    
    visited.insert(std::make_tuple(src.lat, src.lng, initialBombCount));

    while (!stateQueue.empty()) {
        SearchState current = stateQueue.top();
        stateQueue.pop();

        if (current.lat == dst.lat && current.lng == dst.lng) {
            return reconstructPath(current.lat, current.lng, current.bombs, parent, src);
        }
        neighbors(current, dst, stateQueue, thisUF, visited, parent);
    }
    throw RouteError(src, dst);
}

void Map::neighbors(const SearchState &current, const Point &dst, std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> &stateQueue, UnionFind& thisUF, std::set<std::tuple<int,int,int>>& visited, std::map<std::tuple<int,int,int>, std::pair<std::tuple<int,int,int>, char>> &parent) {
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
            if (cellType == '*') {
                newBombCount += 1;
            }
        }
        else if (cellType == '#') {
            if (newBombCount > 0) {
                if (thisUF.shouldBomb(grid[current.lat][current.lng], grid[neighborY][neighborX], grid[dst.lat][dst.lng])) { 
                    //std::cout << "should bomb " << current.lat << " " << current.lng << std::endl;
                    canVisit = true;
                    // bombingSim(grid[neighborY][neighborX], thisUF);
                    newBombCount -= 1;
                    
                }
            }
        }

        if (canVisit) {
            auto neighborState = std::make_tuple(neighborY, neighborX, newBombCount);
            if (visited.count(neighborState) == 0) {
                visited.insert(neighborState);
                parent[neighborState] = {std::make_tuple(current.lat, current.lng, current.bombs), directions[d]};
                SearchState next(neighborY, neighborX, newBombCount);
                stateQueue.push(next);
            }
        }
    }
}

std::string Map::reconstructPath(int goalY, int goalX, int goalBombs, std::map<std::tuple<int,int,int>, std::pair<std::tuple<int,int,int>, char>> &parent, Point src)
{
    std::string path = "";
    int row = goalY;
    int col = goalX;
    int bombs = goalBombs;

    while (!(row == src.lat && col == src.lng)) {
        std::tuple<int,int,int> current = std::make_tuple(row, col, bombs);
        auto iterator = parent.find(current);
        if (iterator == parent.end()) { // if path doesnt exist
            break;
        }
        else {
            char move = iterator->second.second;
            path = move + path;

            std::tuple<int,int,int> prev = iterator->second.first;
            row = std::get<0>(prev);
            col = std::get<1>(prev);
            bombs = std::get<2>(prev);
        }

    }
    return path;
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

