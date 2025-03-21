#include "Map.h"
#include "Errors.h"

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
    
    std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> stateQueue(CompareStates(SearchState(dst.lat, dst.lng, 0, "")));

    // reset visited states
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            grid[i][j].visited = false;
            grid[i][j].prevBombCount= 0;

        }
    }

    UnionFind thisUF = uf;

    if (grid[src.lat][src.lng].type == '*') {
        initialBombCount = 1;
    }

    SearchState initialState(src.lat, src.lng, initialBombCount, "");
    stateQueue.push(initialState);
    
    grid[src.lat][src.lng].visited = true;

    if (initialBombCount > 0) {
        grid[src.lat][src.lng].prevBombCount = 1;

    }

    while (!stateQueue.empty()) {
        SearchState current = stateQueue.top();
        stateQueue.pop();

        if (current.lat == dst.lat && current.lng == dst.lng) {
            return current.route;
        }
        neighbors(current, dst, stateQueue, thisUF);
    }
    throw RouteError(src, dst);
}

void Map::neighbors(const SearchState &current, const Point &dst, std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> &stateQueue, UnionFind& thisUF) {
    for (int d = 0; d < 4; d++) {
        int neighborY = current.lat + dr[d];
        int neighborX = current.lng + dc[d];

        if (neighborY < 0 || neighborX < 0 || neighborX >= columns || neighborY >= rows) {
            continue;
        }

        std::string nextStep;
        
        if (dr[d] == 1) { nextStep = "s"; }
        else if (dr[d] == -1) { nextStep += "n"; }
        else if (dc[d] == 1) { nextStep += "e"; }
        else if (dc[d] == -1) { nextStep += "w"; }

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
                    bombingSim(grid[neighborY][neighborX], thisUF);
                    newBombCount -= 1;
                    
                }
                //else
                    //std::cout << "should NOT bomb " << current.lat << " " << current.lng << std::endl;
            }
        }
        else if (cellType == '~') {
            continue;
        }

        if (canVisit) {
            if (!grid[neighborY][neighborX].visited || (grid[neighborY][neighborX].prevBombCount < newBombCount && newBombCount < maxBombCount && newBombCount < maxBouldersCount)) {
                SearchState next(neighborY, neighborX, newBombCount, current.route+nextStep);
                stateQueue.push(next);
                grid[neighborY][neighborX].visited = true;
                grid[neighborY][neighborX].prevBombCount = newBombCount;
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

void Map::bombingSim(Node boulder, UnionFind& thisUF) {
    int bRow = boulder.y;
    int bCol = boulder.x;
    int boulderIdx = thisUF.getIndex(boulder);

    if (bCol + 1 < columns && isWalkable(grid[bRow][bCol+1])) {
        thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow][bCol+1]));
    }
    if (bCol - 1 >= 0 && isWalkable(grid[bRow][bCol-1])) {
        thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow][bCol-1]));
    }
    if (bRow + 1 < rows && isWalkable(grid[bRow+1][bCol])) {
        thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow+1][bCol]));
    }
    if (bRow - 1 >= 0 && isWalkable(grid[bRow-1][bCol])) {
        thisUF.unite(boulderIdx, thisUF.getIndex(grid[bRow-1][bCol]));
    }
}

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

