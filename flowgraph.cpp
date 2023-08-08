#ifndef FlowGraph_H
#define FlowGraph_H
#include <edgenode.cpp>
#include <cmath>
#include <vector>
#include <QSet>
#define	MAXV 100

struct FlowGraph
{
    EdgeNode *edges[MAXV+1];
    int degree[MAXV+1];
    int nvertices;
    int nedges;
    int r = 50;
    EdgeNode vertexCoord[MAXV];

    FlowGraph(int nvertices)
    {
        this->nvertices = nvertices;
        for (int i=0; i<MAXV; i++)
        {
            this->degree[i] = 0;
            this->edges[i] = NULL;
        }
    }

    void GetCoordsArray()
    {
        double h = 375;
        double k = 200;
        double a = 325;
        double b = 175;
        double angleIncrement = 2 * M_PI / nvertices;
        double currentAngle = 0;
        double x = h - a;
        double y = k;

        std::vector<double> coords;
        coords.push_back(x);
        coords.push_back(y);
        for (int i = 1; i < nvertices; ++i)
        {
            currentAngle += angleIncrement;
            x = h - a * cos(currentAngle);
            y = k - b * sin(currentAngle);
            coords.push_back(x);
            coords.push_back(y);
        }
        for (int i = 0, j = 0; i < nvertices * 2; ++i, ++j)
        {
            vertexCoord[i].x = coords[j++];
            vertexCoord[i].y = coords[j];
        }
    }

    void FindSourceAndTarget(int &source, int &target)
    {
        QSet<int> sourceSet;
        for (int i=1; i<=nvertices; i++)
        {
            sourceSet.insert(i);
        }
        EdgeNode *p;
        for (int i=1; i<=nvertices; i++)
        {
            p = edges[i];
            bool isTarget = true;
            while (p != NULL)
            {
                isTarget = false;
                sourceSet.remove(p->v);
                p = p->next;
            }
            if (isTarget)
            {
                target = i;
            }
        }
        source = *(sourceSet.begin());
    }
};

#endif
