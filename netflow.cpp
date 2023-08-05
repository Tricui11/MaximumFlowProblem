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

    void initialize_graph(FlowGraph *g)
    {
        int i;

        g -> nvertices = 0;
        g -> nedges = 0;

        for (i=0; i<MAXV; i++) g->degree[i] = 0;
        for (i=0; i<MAXV; i++) g->edges[i] = NULL;
    }

    void read_flow_graph(FlowGraph *g, bool directed)
    {
        int i;				/* counter */
        int m;				/* number of edges */
        int x,y,w;			/* placeholder for edge and weight */

        initialize_graph(g);



        g->nvertices = 8;
        m = 13;

        for (i=1; i<=m; i++)
        {
            if (i == 1)
            {
                x = 1; y = 2; w = 6;
            }
            else if (i == 2)
            {
                x = 1; y = 3; w = 6;
            }
            else if (i == 3)
            {
                x = 3; y = 2; w = 5;
            }
            else if (i == 4)
            {
                x = 2; y = 4; w = 4;
            }
            else if (i == 5)
            {
                x = 2; y = 5; w = 2;
            }
            else if (i == 6)
            {
                x = 3; y = 5; w = 9;
            }
            else if (i == 7)
            {
                x = 5; y = 4; w = 8;
            }
            else if (i == 8)
            {
                x = 5; y = 7; w = 7;
            }
            else if (i == 9)
            {
                x = 4; y = 7; w = 2;
            }
            else if (i == 10)
            {
                x = 4; y = 6; w = 4;
            }
            else if (i == 11)
            {
                x = 7; y = 6; w = 11;
            }
            else if (i == 12)
            {
                x = 6; y = 8; w = 7;
            }
            else if (i == 13)
            {
                x = 7; y = 8; w = 4;
            }





            insert_flow_edge(g, x, y, directed, w);
        }
    }

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

    void print_flow_graph(FlowGraph *g)
    {
        int i;
        EdgeNode *p;

        for (i=1; i<=g->nvertices; i++)
        {
            printf("%d: ",i);
            p = g->edges[i];
            while (p != NULL)
            {
                printf(" %d(%d,%d)",p->v, p->capacity, p->flow);
                p = p->next;
            }
            printf("\n");
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
