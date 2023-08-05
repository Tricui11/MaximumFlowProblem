#ifndef GRAPHS_H
#define GRAPHS_H
#include <cmath>
#include <vector>

struct Coords {
public:
    int x;
    int y;
};

class Graph {
public:
    Graph(int capacity)
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
    Coords vertexCoord[12];
};




#endif // GRAPHS_H
