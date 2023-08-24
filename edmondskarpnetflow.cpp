#include <basenetflow.cpp>

class EdmondsKarpNetflow : public BaseNetflow
{
public:
    mutable bool processed[MAXV+1];
    mutable bool discovered[MAXV+1];
    mutable int parent[MAXV+1];

    EdgeNode* insert_flow_edge(FlowGraph *g, int x, int y, bool directed, int w) const override
    {
        EdgeNode* p = BaseNetflow::insert_flow_edge(g, x, y, directed, w);
        g->edges[x] = p;
        g->degree[x]++;
        return p;
    }

    void bfs(FlowGraph *g, int start) const override
    {
        Queue q;
        int v;
        int y;
        EdgeNode *p;

        q.init_queue();
        q.enqueue(start);
        discovered[start] = true;

        while (q.count > 0)
        {
            v = q.dequeue();
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
                            q.enqueue(y);
                            discovered[y] = true;
                            parent[y] = v;
                        }
                    }
                }
                p = p->next;
            }
        }
    }

    void initialize_search(FlowGraph *g) const
    {
        for (int i=1; i<=g->nvertices; i++)
        {
            processed[i] = false;
            discovered[i] = false;
            parent[i] = -1;
        }
    }

    EdgeNode* find_edge(FlowGraph *g, int x, int y) const override
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

    void add_residual_edges(FlowGraph *g) const
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

    int path_volume(FlowGraph *g, int start, int flow, int end, int parents[]) const override
    {
        if (parents[end] == -1) return(0);

        EdgeNode *e = find_edge(g,parents[end],end);
        if (start == parents[end]) return(e->residual);
        else return(std::min(path_volume(g, start, flow, parents[end], parents), e->residual));
    }

    void augment_path(FlowGraph *g, int start, int end, int parents[], int volume) const
    {
        if (start == end) return;

        EdgeNode *e = find_edge(g, parents[end], end);
        e->flow += volume;
        e->residual -= volume;
        e = find_edge(g, end, parents[end]);
        e->residual += volume;

        augment_path(g, start, parents[end], parents, volume);
    }

    int netflow(FlowGraph *g, int source, int target) const override
    {
        add_residual_edges(g);
        initialize_search(g);
        bfs(g, source);
        int volume = path_volume(g, source, 0, target, parent);
        int total = volume;
        while (volume > 0)
        {
            augment_path(g, source, target, parent, volume);
            initialize_search(g);
            bfs(g, source);
            volume = path_volume(g, source, 0, target, parent);
            total += volume;
        }

        return total;
    }
};
