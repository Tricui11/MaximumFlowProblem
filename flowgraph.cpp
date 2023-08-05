#include <edgenode.cpp>
#define	MAXV 100

struct FlowGraph
{
    EdgeNode *edges[MAXV+1];
    int degree[MAXV+1];
    int nvertices;
    int nedges;
};
