struct Node {
    char type;
    int y;
    int x;

    Node(){}

    Node(char c, int yc, int xc) {
        type = c; // #, ~, ., *
        y = yc;
        x = xc;
    }
    
    // ~Node();

};
