#ifndef BASENETFLOW_H
#define BASENETFLOW_H

#include <queue.cpp>
#include <flowgraph.cpp>
#include <edgenode.cpp>
#include <algorithm>

class BaseNetflow
{
public:
    virtual EdgeNode* insert_flow_edge(FlowGraph *g, int x, int y, bool directed, int w) const
    {
        EdgeNode *p;
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        p->v = y;
        p->capacity = w;
        p->flow = 0;
        p->residual = w;
        p->rev = g->degree[y];
        p->next = g->edges[x];        

        if (directed == false)
        {
            insert_flow_edge(g, y, x, true, w);
        }
        else
        {
            g->nedges++;
        }

        return p;
    }

    virtual void bfs(FlowGraph *g, int start) const {}

    virtual EdgeNode* find_edge(FlowGraph *g, int x, int y) const {}

    virtual int path_volume(FlowGraph *g, int start, int flow, int end, int parents[]) const {}

    virtual int netflow(FlowGraph *g, int source, int target) const {}
};

#endif
