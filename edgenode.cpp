#ifndef EDGENODE_H
#define EDGENODE_H

struct EdgeNode
{
    int x;
    int y;
    int v;
    int capacity;
    int flow;
    int residual;
    int rev;
    EdgeNode *next;
};

#endif // EDGENODE_H
