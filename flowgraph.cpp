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

    FlowGraph()
    {
    }

    FlowGraph(int capacity)
    {
        std::vector<double> coords = GetCoordsArray();;
        this->capacity = capacity;
        for (int i = 0, j = 0; i < capacity * 2; ++i, ++j)
        {
            vertexCoord[i].x = coords[j++];
            vertexCoord[i].y = coords[j];
        }
    }


    std::vector<double> GetCoordsArray()
    {
        double h = 375;
        double k = 200;
        double a = 325;
        double b = 175;
        int N = 10;

        double angleIncrement = 2 * M_PI / N;
        double currentAngle = 0;

        std::vector<double> result;

        // Первая точка V1 на большой полуоси
        double x = h + a;
        double y = k;

        result.push_back(x);
        result.push_back(y);

        for (int i = 1; i < N; ++i) {
            currentAngle += angleIncrement;

            x = h + a * cos(currentAngle);
            y = k + b * sin(currentAngle);

            result.push_back(x);
            result.push_back(y);
        }

        return result;
    }


    int r = 50;
    int capacity;
    EdgeNode vertexCoord[12];
};

#endif
