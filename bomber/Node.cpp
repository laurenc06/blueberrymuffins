class Node {
    char type;
    int y;
    int x;

    Node(){}

    Node(char c, int yc, int xc) {
        type = c; // #, ~, ., *
        y = yc;
        x = xc;
    }

    Node(Node& other) {
        type = other.type;
        y = other.y;
        x = other.x;
    }
    
    // ~Node();

};
