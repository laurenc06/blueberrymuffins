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

    if (visitedStates) {
        for (int i = 0; i < rows; ++i) {
            if (visitedStates[i]) {
                for (int j = 0; i < columns; ++j) {
                    delete[] visitedStates[i][j];
                }
                delete[] visitedStates[i];
            }
        }
        delete[] visitedStates;
        visitedStates = nullptr;
    }

    columns = 0;
    rows = 0;
}

Map::Map(std::istream& stream) {
    std::vector<std::string> lines;
    std::string line;
    while (getline(stream, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    rows = static_cast<int>(lines.size());
    columns = static_cast<int>(lines[0].length());

    grid = new Node*[rows]();
    visitedStates = new bool**[rows];

    for (int i = 0; i < rows; i++) {
        grid[i] = new Node[columns];
        visitedStates[i] = new bool*[columns];
        for (int j = 0; j < columns; j++) {
            visitedStates[i][j] = new bool[2];
            visitedStates[i][j][0] = false; // represents bomb used state
            visitedStates[i][j][1] = false; // bomb available state
        }
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            grid[y][x] = Node(lines[y][x], y, x);
        }
    }

    uf.setCols(columns);

    // loop over every cell and call uf.insert
}

std::string Map::route(Point src, Point dst) {
    fin = dst;
    int initialBombCount = 0;

    if (grid[src.lat][src.lng].type == '*') {
        initialBombCount = 1;
    }
    
    std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> stateQueue(CompareStates(SearchState(dst.lat, dst.lng, 0, "")));

    // reset visited states
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            visitedStates[i][j][0] = false;
            visitedStates[i][j][1] = false;
        }
    }

    if(!CheckStartPoint(src)) {
        throw PointError(src);
    }
    else if(!CheckEndPoint(dst)) {
        throw PointError(dst);
    }

    SearchState initialState(src.lat, src.lng, initialBombCount, "");
    stateQueue.push(initialState);
    
    visitedStates[src.lat][src.lng][0] = true;

    if (initialBombCount > 0) {
        visitedStates[src.lat][src.lng][1] = true; // true or false
    }

    while (!stateQueue.empty()) {
        SearchState current = stateQueue.top();
        stateQueue.pop();

        if (current.lat == dst.lat && current.lng == dst.lng) {
            return current.route;
        }
        neighbors(current, dst, stateQueue);
    }
    throw RouteError(src, dst);
}

void Map::neighbors(const SearchState &current, const Point &dst, std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> &stateQueue) {
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
                if (uf.shouldBomb(grid[current.lat][current.lng], grid[neighborY][neighborX], grid[dst.lat][dst.lng])) { 
                    canVisit = true;
                    newBombCount -= 1;
                }
            }
        }
        else if (cellType == '~') {
            continue;
        }

        if (canVisit) {
            if (!visitedStates[neighborY][neighborX][newBombCount]) {
                SearchState next(neighborY, neighborX, newBombCount, current.route+nextStep);
                stateQueue.push(next);
                visitedStates[neighborY][neighborX][newBombCount] = true;
            }
        }
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

