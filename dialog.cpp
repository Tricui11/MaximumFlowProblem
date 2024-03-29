#include <dialog.h>
#include <ui_dialog.h>
#include <QMessageBox>
#include <QButtonGroup>
#include <QPainter>
#include <QRegularExpression>
#include <cmath>
#include <iostream>
#include <edmondskarpnetflow.cpp>
#include <dinicnetflow.cpp>
#include <QtMath>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QRegularExpression cellRegEx("^(?!0)\\d*$");
    cellValidator = new QRegularExpressionValidator(cellRegEx, this);
    ui->lineEdit_number_of_vertices->setValidator(cellValidator);
    ui->lineEdit_vertex1->setValidator(cellValidator);
    ui->lineEdit_vertex_2->setValidator(cellValidator);
    ui->lineEdit_weight->setValidator(cellValidator);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioButton_EdmondsKarp);
    buttonGroup->addButton(ui->radioButton_Dinic);
    ui->radioButton_EdmondsKarp->setChecked(true);
}

Dialog::~Dialog()
{
    delete ui;
}

int matrix[MAXV][MAXV];
FlowGraph* graphUI;
int r = 50;

void Dialog::paintEvent(QPaintEvent *event)
{
    if (graphUI == nullptr)
    {
        return;
    }
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    if (graphUI->nvertices == MAXV) return;
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::black);
    painter.setFont(QFont("times", 22));
    int xOffset;
    for (int i = 0; i < graphUI->nvertices; ++i)
    {
        for (int j = i+1; j < graphUI->nvertices; ++j)
        {
            if (matrix[i][j] != 0)
            {
                drawLineWithArrow(painter, QPoint(circles[i].position.x(), circles[i].position.y()), QPoint(circles[j].position.x(), circles[j].position.y()));
            }
            if (matrix[j][i] != 0)
            {
                drawLineWithArrow(painter, QPoint(circles[j].position.x(), circles[j].position.y()), QPoint(circles[i].position.x(), circles[i].position.y()));
            }
        }
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.drawEllipse(circles[i].position.x() - circles[i].radius/2, circles[i].position.y() - circles[i].radius/2, circles[i].radius, circles[i].radius);
        painter.setFont(QFont("times", 22));
        xOffset = r/5;
        if (i >= 9) xOffset += r/10;
        painter.drawText(circles[i].position.x() - xOffset, circles[i].position.y() + r/5, QString::number(i+1));
    }
}

void Dialog::drawLineWithArrow(QPainter& painter, QPoint start, QPoint end)
{
    qreal arrowSize = 20;
    painter.setBrush(Qt::black);
    QLineF line(end, start);
    double angle = std::atan2(-line.dy(), line.dx());
    QPoint vector = end - start;
    double length = sqrt(vector.x() * vector.x() + vector.y() * vector.y());
    double xOffset = r/2*((double)vector.x()) / length;
    double yOffset = r/2*((double)vector.y()) / length;
    QPoint offset(xOffset, yOffset);
    QPoint endpoint = end - offset;
    QPointF arrowP1 = endpoint + QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
    QPointF arrowP2 = endpoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
    QPolygonF arrowHead;
    arrowHead.clear();
    arrowHead << endpoint << arrowP1 << arrowP2;
    painter.drawLine(endpoint, start);
    painter.drawPolygon(arrowHead);
}

void Dialog::on_pushButton_draw_vertices_clicked()
{
    ui->plainTextEdit_reslut->setPlainText("");
    for (int i = 0; i < MAXV; ++i)
    {
        for (int j = 0; j < MAXV; ++j)
        {
            matrix[i][j] = 0;
        }
    }
    int nvertices = ui->lineEdit_number_of_vertices->text().toInt();
    if (graphUI != nullptr)
    {
        delete graphUI;
    }
    graphUI = new FlowGraph(MAXV);
    if(nvertices != graphUI->nvertices)
    {
        if (nvertices > MAXV)
        {
            QMessageBox::critical(this, "Ошибка", "Введенное число вершин превышает допустимое значение!");
            return;
        }
        graphUI->nvertices = nvertices;
        graphUI->GetCoordsArray();
    }
    circles.clear();
    for (int i = 0; i < graphUI->nvertices; ++i)
    {
        circles.append(Circle(QPointF(graphUI->edges[i]->x, graphUI->edges[i]->y), r));
    }
    repaint();
}

void Dialog::on_pushButton_add_clicked()
{
    QString str_vertex1 = ui->lineEdit_vertex1->text();
    QString str_vertex2 = ui->lineEdit_vertex_2->text();
    QString str_weight = ui->lineEdit_weight->text();
    int weight = str_weight.toInt();
    int vertex1 = -1;
    int vertex2 = -1;
    for (int i = 0; i < graphUI->nvertices; ++i)
    {
        if (str_vertex1 == QString::number(i+1))
        {
            vertex1 = i;
        }
        if (str_vertex2 == QString::number(i+1))
        {
            vertex2 = i;
        }
    }
    if (vertex1 == -1 || vertex2 == -1 || vertex1 == vertex2)
    {
        QMessageBox::critical(this, "Ошибка", "Неверно введены вершины");
    }
    else
    {
        matrix[vertex1][vertex2] = weight;
        QString leText = ui->plainTextEdit_reslut->toPlainText();
        leText.append("Edge: (%1, %2), capacity:%3");
        leText = leText.arg(vertex1+1).arg(vertex2+1).arg(weight);
        leText.append("\n");
        ui->plainTextEdit_reslut->setPlainText(leText);
        repaint();
    }
}

void Dialog::on_pushButton_FindFlow_clicked()
{
    int source, target;
    findSourceAndTarget(source, target);

    FlowGraph* graph = new FlowGraph(graphUI->nvertices);
    BaseNetflow* netflow;

    if (ui->radioButton_EdmondsKarp->isChecked())
    {
        netflow = new EdmondsKarpNetflow();
        for (int i = 0; i < graph->nvertices; ++i)
        {
            for (int j = i + 1; j < graph->nvertices; ++j)
            {
                if (matrix[i][j] != 0)
                {
                    netflow->insert_flow_edge(graph, i+1, j+1, true, matrix[i][j]);
                }
                if (matrix[j][i] != 0)
                {
                    netflow->insert_flow_edge(graph, j+1, i+1, true, matrix[j][i]);
                }
            }
        }
        int total = netflow->netflow(graph, source+1, target+1);
        print_result(total, graph, true);
    }
    else
    {
        netflow = new DinicNetflow();
        for (int i = 0; i < graph->nvertices; ++i)
        {
            for (int j = i+1; j < graph->nvertices; ++j)
            {
                if (matrix[i][j] != 0)
                {
                    netflow->insert_flow_edge(graph, i, j, true, matrix[i][j]);
                    netflow->insert_flow_edge(graph, j, i, true, 0);
                }
                if (matrix[j][i] != 0)
                {
                    netflow->insert_flow_edge(graph, j, i, true, matrix[j][i]);
                    netflow->insert_flow_edge(graph, i, j, true, 0);
                }
            }
        }
        int total = netflow->netflow(graph, source, target);
        print_result(total, graph, false);
    }

    delete graph;
    delete netflow;
}

void Dialog::findSourceAndTarget(int &source, int &target)
{
    std::vector<bool> indexesSource(graphUI->nvertices, true);
    std::vector<bool> indexesTarget(graphUI->nvertices, true);
    for (int i = 0; i < graphUI->nvertices; ++i)
    {
        for (int j = i+1; j < graphUI->nvertices; ++j)
        {
            if (matrix[i][j] != 0)
            {
                indexesSource[j] = false;
                indexesTarget[i] = false;
            }
            if (matrix[j][i] != 0)
            {
                indexesSource[i] = false;
                indexesTarget[j] = false;
            }
        }
    }
    int i;
    for(i = 0; i < graphUI->nvertices; ++i)
    {
        if (indexesSource[i])
        {
            source = i;
            break;
        }
    }
    for(i = 0; i < graphUI->nvertices; ++i)
    {
        if (indexesTarget[i])
        {
            target = i;
            break;
        }
    }
}

void Dialog::print_result(int total, FlowGraph* g, bool isPrintPath)
{
    EdgeNode *p;
    QString result;
    if (isPrintPath)
    {
        for (int i=1; i<=g->nvertices; i++)
        {
            p = g->edges[i];
            while (p != NULL)
            {
                QString edgeInfo = "";
                if (p->capacity > 0)
                {
                    edgeInfo.append("Edge: V%1-V%2, C: %3, F: %4");
                    edgeInfo = edgeInfo.arg(i).arg(p->v).arg(p->capacity).arg(p->flow);
                    edgeInfo.append("\n");
                }
                p = p->next;
                if (edgeInfo != "")
                {
                    result.append(edgeInfo);
                }
            }
        }
        result.append("\n");
    }
    result.append("Total flow = %1\n");
    result = result.arg(total);
    ui->plainTextEdit_reslut->setPlainText(result);
}

void Dialog::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0; i < circles.size(); ++i)
    {
        if (event->button() == Qt::LeftButton &&
            qPow(event->x() - circles[i].position.x(), 2) + qPow(event->y() - circles[i].position.y(), 2) <= qPow(circles[i].radius / 2, 2))
        {
            circles[i].isDragging = true;
        }
    }
}

void Dialog::mouseMoveEvent(QMouseEvent *event)
{
    for (int i = 0; i < circles.size(); ++i)
    {
        if (circles[i].isDragging)
        {
            circles[i].position = event->pos();
            update();
        }
    }
}

void Dialog::mouseReleaseEvent(QMouseEvent *event)
{
    for (int i = 0; i < circles.size(); ++i)
    {
        if (event->button() == Qt::LeftButton && circles[i].isDragging)
        {
            circles[i].isDragging = false;
        }
    }
}
