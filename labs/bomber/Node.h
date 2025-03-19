#ifndef NODE_H
#define NODE_H

class Node {
    public: 
        char type;
        int y;
        int x;
        bool visited = false;
        int prevBombCount = 0;

    Node(){}

    Node(char c, int yc, int xc) {
        type = c; // #, ~, ., *
        y = yc;
        x = xc;
    }

    Node(const Node& other) {
        type = other.type;
        y = other.y;
        x = other.x;
        visited = other.visited;
        prevBombCount = other.prevBombCount;
    }

    Node& operator=(const Node& other) {
        if (this != & other) {
            type = other.type;
            y = other.y;
            x = other.x;
            visited = other.visited;
            prevBombCount = other.prevBombCount;
        }
        return *this;
    }
    
    // ~Node();

};

#endif
