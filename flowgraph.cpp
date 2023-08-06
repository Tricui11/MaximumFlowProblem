#ifndef FlowGraph_H
#define FlowGraph_H
#include <edgenode.cpp>
#include <cmath>
#include <vector>
#define	MAXV 100

struct FlowGraph
{
    EdgeNode *edges[MAXV+1];
    int degree[MAXV+1];
    int nvertices;
    int nedges;

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

        std::vector<double> coords;

        // Первая точка V1 на большой полуоси
        double x = h - a;
        double y = k;

        coords.push_back(x);
        coords.push_back(y);

        for (int i = 1; i < nvertices; ++i) {
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

    int r = 50;
    EdgeNode vertexCoord[MAXV];
};

#endif
