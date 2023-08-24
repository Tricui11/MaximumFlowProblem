#include <algorithm>
#include <basenetflow.cpp>

class DinicNetflow : public BaseNetflow
{
public:
    EdgeNode* insert_flow_edge(FlowGraph *g, int x, int y, bool directed, int w) const override
    {
        EdgeNode* p = BaseNetflow::insert_flow_edge(g, x, y, directed, w);
        g->edges[x][g->degree[x]] = *p;
        g->degree[x]++;
        return p;
    }

    void bfs(FlowGraph* g, int start) const override
    {
        Queue q;
        int v;

        for (int i = 0; i < g->nvertices; i++)
        {
            g->level[i] = -1;
        }
        g->level[start] = 0;

        q.init_queue();
        q.enqueue(start);

        while (q.count > 0)
        {
            v = q.dequeue();
            for (int i = 0; i < g->degree[v]; ++i)
            {
                EdgeNode& e = g->edges[v][i];
                if (g->level[e.v] < 0 && e.flow < e.capacity)
                {
                    g->level[e.v] = g->level[v] + 1;
                    q.enqueue(e.v);
                }
            }
        }
    }

    EdgeNode* find_edge(FlowGraph *g, int x, int y) const override
    {
        return &(g->edges[x][y]);
    }

    int path_volume(FlowGraph* g, int start, int flow, int target, int parents[]) const override
    {
        if (start == target) return flow;

        for (; parents[start] < g->degree[start]; ++parents[start])
        {
            EdgeNode* e = find_edge(g, start, parents[start]);
            if (g->level[e->v] == g->level[start] + 1 && e->flow < e->capacity)
            {
                int curr_flow = fmin(flow, e->capacity - e->flow);
                int temp_flow = path_volume(g, e->v, curr_flow, target, parents);

                if (temp_flow > 0)
                {
                    e->flow += temp_flow;
                    g->edges[e->v][e->rev].flow -= temp_flow;
                    return temp_flow;
                }
            }
        }

        return 0;
    }

    int netflow(FlowGraph *g, int source, int target) const override
    {
        int total = 0;
        while (g->level[target] >= 0)
        {
            int* start = new int[g->nvertices + 1] { };
            bfs(g, source);
            int volume = path_volume(g, source, INT_MAX, target, start);
            total += volume;
            while (volume > 0)
            {
                volume = path_volume(g, source, INT_MAX, target, start);
                total += volume;
            }

            delete[] start;
        }

        return total;
    }
};
