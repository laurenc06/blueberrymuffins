#include "Map.h"
#include "Errors.h"

// Member Functions
Map::Node::Node() {
}

Map::Node::Node(char c, int yc, int xc) {
    type = c; // #, ~, ., *
    y = yc;
    x = xc;
}

Map::~Map() {
    for (int i =0; i < rows; ++i) {
        delete[] arr[i];
    }
    delete[] arr;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; i < columns; ++j) {
            delete[] visited[i][j];
        }
        delete[] visited[i];
    }
    delete[] visited;

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
    rows = lines.size();
    columns = lines[0].length();

    arr = new Node*[rows]();
    visited = new bool**[rows];

    for (int i = 0; i < rows; i++) {
        arr[i] = new Node[columns];
        visited[i] = new bool*[columns]();
        for (int j = 0; j < columns; j++) {
            visited[i][j] = new bool[2];
            visited[i][j][0] = false; // represents bomb used state
            visited[i][j][1] = false; // bomb available state
        }
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            arr[y][x] = Node(lines[y][x], y, x);
        }
    }
}

std::string Map::route(Point src, Point dst) {
    // std::cout << "rows: " << rows << " columns: " << columns << std::endl;
    fin = dst;
    int initialBomb = 0;

    if (arr[src.lat][src.lng].type == '*') {
        initialBomb = 1;
    }
    
    std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> pq(CompareStates(SearchState(dst.lat, dst.lng, 0, "")));

    // reset
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            visited[i][j][0] = false;
            visited[i][j][1] = false;
        }
    }

    if(!CheckStartPoint(src)) {
        throw PointError(src);
    }
    else if(!CheckEndPoint(dst)) {
        throw PointError(dst);
    }

    SearchState start(src.lat, src.lng, initialBomb, "");
    pq.push(start);
    
    visited[src.lat][src.lng][0] = true;
    if (initialBomb > 0) {
        visited[src.lat][src.lng][1] = true; // true or false
    }

    while (!pq.empty()) {
        SearchState current = pq.top();
        pq.pop();

        if (current.lat == dst.lat && current.lng == dst.lng) {
            return current.route;
        }
        neighbors(current, dst, pq);
    }
    throw RouteError(src, dst);
}

void Map::neighbors(const SearchState &current, const Point &dst, std::priority_queue<SearchState, std::vector<SearchState>, CompareStates> &pq) {
    int neighborY, neighborX;
    for (int d = 0; d < 4; d++) {
        neighborY = current.lat + dr[d];
        neighborX = current.lng + dc[d];

        if (neighborY < 0 || neighborX < 0 || neighborX > columns - 1 || neighborY > rows - 1) {
            continue;
        }

        std::string nextStep;
        
        if (dr[d] == 1) { nextStep = "s"; }
        else if (dr[d] == -1) { nextStep += "n"; }
        else if (dc[d] == 1) { nextStep += "e"; }
        else if (dc[d] == -1) { nextStep += "w"; }

        char thisType = arr[neighborY][neighborX].type;
        int newBombState = current.bombs;
        bool canVisit = false;

        if (thisType == '.' || thisType == '*') {
            canVisit = true;
            if (thisType == '*') {
                newBombState += 1;
            }
        }
        else if (thisType == '#') {
            if (current.bombs > 0) {
                if (uf.shouldBomb(current.lat, current.lng, neighborY, neighborX, dst)) { 
                    canVisit = true;
                    newBombState -= 1;
                }
            }
        }
        else if (thisType == '~') {
            continue;
        }
        if (canVisit) {
            int BombIndex = newBombState;
            if (!visited[neighborY][neighborX][BombIndex]) {
                SearchState next(neighborY, neighborX, newBombState, current.route+nextStep);
                pq.push(next);
                visited[neighborY][neighborX][BombIndex] = true;
            }
        }
    }
}

bool Map::CheckStartPoint(Point start) {
    if (start.lat < 0 || start.lng < 0 || start.lat > rows - 1 || start.lng > columns - 1) {
        return false;
    }
    if (arr[start.lat][start.lng].type != '.' && arr[start.lat][start.lng].type != '*') {
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

