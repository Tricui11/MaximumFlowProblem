#include <queue.cpp>
#include <flowgraph.cpp>
#include <edgenode.cpp>
#include <algorithm>

class Netflow
{
public:
    bool processed[MAXV+1];
    bool discovered[MAXV+1];
    int parent[MAXV+1];

    void insert_flow_edge(FlowGraph *g, int x, int y, bool directed, int w)
    {
        EdgeNode *p;
        p = (EdgeNode*)malloc(sizeof(EdgeNode));
        p->v = y;
        p->capacity = w;
        p->flow = 0;
        p->residual = w;
        p->next = g->edges[x];

        g->edges[x] = p;
        g->degree[x]++;

        if (directed == false)
        {
            insert_flow_edge(g, y, x, true, w);
        }
        else
        {
            g->nedges++;
        }
    }

    EdgeNode* find_edge(FlowGraph *g, int x, int y)
    {
        EdgeNode *p;
        p = g->edges[x];
        while (p != NULL)
        {
            if (p->v == y)
            {
                return(p);
            }
            else
            {
                p = p->next;
            }
        }

        return(NULL);
    }

    void add_residual_edges(FlowGraph *g)
    {
        int i;
        EdgeNode *p;

        for (i=1; i<=g->nvertices; i++)
        {
            p = g->edges[i];
            while (p != NULL)
            {
                if (find_edge(g,p->v,i) == NULL)
                {
                    insert_flow_edge(g, p->v, i, true, 0);
                }
                p = p->next;
            }
        }
    }

    void initialize_search(FlowGraph *g)
    {
        for (int i=1; i<=g->nvertices; i++)
        {
            processed[i] = false;
            discovered[i] = false;
            parent[i] = -1;
        }
    }

    void bfs(FlowGraph *g, int start)
    {
        Queue q;
        int v;
        int y;
        EdgeNode *p;

        init_queue(&q);
        enqueue(&q,start);
        discovered[start] = true;







        while (q.count > 0)
        {
            v = dequeue(&q);
            processed[v] = true;
            p = g->edges[v];
            while (p != NULL)
            {
                y = p->v;
                if (p->residual > 0)
                {
                    if (processed[y] == false /*|| g->directed*/)
                    {
                        if (discovered[y] == false)
                        {
                            enqueue(&q,y);
                            discovered[y] = true;
                            parent[y] = v;
                        }
                    }
                }
                p = p->next;
            }
        }
    }

    int path_volume(FlowGraph *g, int start, int end, int parents[])
    {
        EdgeNode *e;

        if (parents[end] == -1) return(0);

        e = find_edge(g,parents[end],end);

        if (start == parents[end])
            return(e->residual);
        else
            return(std::min(path_volume(g,start,parents[end],parents), e->residual) );
    }

    void augment_path(FlowGraph *g, int start, int end, int parents[], int volume)
    {
        EdgeNode *e;

        if (start == end) return;

        e = find_edge(g,parents[end],end);
        e->flow += volume;
        e->residual -= volume;

        e = find_edge(g,end,parents[end]);
        e->residual += volume;

        augment_path(g,start,parents[end],parents,volume);
    }

    void netflow(FlowGraph *g, int source, int sink)
    {
        int volume;

        add_residual_edges(g);

        initialize_search(g);
        bfs(g, source);

        volume = path_volume(g, source, sink, parent);

        while (volume > 0)
        {
            augment_path(g, source, sink, parent, volume);
            initialize_search(g);
            bfs(g, source);
            volume = path_volume(g, source, sink, parent);
        }
    }
};
