#include <QPointF>

class Circle
{
public:
    QPointF position;
    int radius;
    bool isDragging;

    Circle(const QPointF& pos, int r) : position(pos), radius(r), isDragging(false) {}
};
