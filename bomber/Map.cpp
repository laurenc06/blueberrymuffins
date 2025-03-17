#include "Map.h"
#include "Errors.h"

// Member Functions
Map::Node::Node() {
}

Map::Node::Node(char c) {
    type = c; // #, ~, ., *
}

Map::~Map() {
    for (int i =0; i < rows; ++i) {
        delete[] arr[i];
        delete[] visited[i];
    }
    delete[] visited;
    delete[] arr;

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
    visited = new bool*[rows];

    for (int i = 0; i < rows; i++) {
        arr[i] = new Node[columns];
        visited[i] = new bool[columns]();
    }

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            arr[y][x] = Node(lines[y][x]);
        }
    }
}

std::string Map::route(Point src, Point dst) {
    // std::cout << "rows: " << rows << " columns: " << columns << std::endl;
    fin = dst;
    
    std::priority_queue<Point, std::vector<Point>, ComparePoint> pq{ComparePoint(fin)};

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            visited[i][j] = false;
        }
    }

    if(!CheckStartPoint(src)) {
        throw PointError(src);
    }
    else if(!CheckEndPoint(dst)) {
        throw PointError(dst);
    }

    Point current, last;
    std::string myRoute = "";

    pq.push(src);
    visited[src.lat][src.lng] = true;

    current = src;

    while (!pq.empty()) {
        last = current;
        current = pq.top();
        pq.pop();

        if (current.lat > last.lat) { myRoute += "s"; }
        else if (current.lat  <last.lat) { myRoute += "n"; }
        else if (current.lng > last.lng) { myRoute += "e"; }
        else if (current.lng < last.lng) { myRoute += "w"; }

        if (current.lat == dst.lat && current.lng == dst.lng) {
            return myRoute;
        }
        neighbors(current, pq);
    }
    throw RouteError(src, dst);
}

void Map::neighbors(Point current, std::priority_queue<Point, std::vector<Point>, ComparePoint> &pq) {
    int neighborY, neighborX;
    for (int d = 0; d < 4; d++) {
        neighborY = current.lat + dr[d];
        neighborX = current.lng + dc[d];

        if (neighborY < 0 || neighborX < 0 || neighborX > columns - 1 || neighborY > rows - 1) {
            continue;
        }
        else {
            if (!(visited[neighborY][neighborX]) && (arr[neighborY][neighborX].type) != '#' && (arr[neighborY][neighborX].type) != '~' ) {
                Point add(neighborY, neighborX);
                pq.push(add); 
                visited[neighborY][neighborX] = true;
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

