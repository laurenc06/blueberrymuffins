<<<<<<< HEAD
#include "UnionFind.h"

UnionFind::UnionFind()
{
}

void UnionFind::insert(int x, Point p)
{
    points[x] = p;
    parents.push_back(x);
    visited.push_back(false);
}

int UnionFind::find(int x)
{
    if(parents[x] != x)
        parents[x] = find(parents[x]);
    return parents[x];
}

void UnionFind::unite(int a, int b)
{
    int aRoot = find(a);
    int bRoot = find(b);
    if(aRoot!=bRoot)
        parents[aRoot] = bRoot; //doesn't matter which one becomes root (?)
    return;
}
void UnionFind::connectPoints()
{
    if(points.size()==0)
        return;
    Point current = points[0];
}

//add new var to count ins
//add after line 45 in Map.cpp
//(UnionFindObjName).insert(ins,Point(y,x))
//increment ins after every insert
=======
#include <vector>
using namespace std;

class UnionFind {
    vector<int> parents;

    public: 
        UnionFind () {
            
        }

        Node* find() {
    
        }

        void union(Node* parent1, Node* parent2) {
            
        }

}
>>>>>>> 0676f1bc57452a3fc4ffa8609095c6b10840e182
