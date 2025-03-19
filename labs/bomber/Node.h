#ifndef NODE_H
#define NODE_H

class Node {
    public: 
        char type;
        int y;
        int x;

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
    }

    Node& operator=(const Node& other) {
        if (this != & other) {
            type = other.type;
            y = other.y;
            x = other.x;
        }
        return *this;
    }
    
    // ~Node();

};

#endif
