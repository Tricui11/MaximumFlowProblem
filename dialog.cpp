#include <dialog.h>
#include <ui_dialog.h>
#include <QMessageBox>
#include <cmath>
#include <QPainter>
#include <QRegularExpression>
#include <netflow.cpp>

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
}

Dialog::~Dialog()
{
    delete ui;
}

int matrix[MAXV][MAXV];
FlowGraph graph(MAXV);
Netflow *netflow = new Netflow();

FlowGraph getGraph()
{
    return graph;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    if (graph.nvertices == MAXV) return;
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::black);
    painter.setFont(QFont("times", 22));
    for (int i = 0; i < graph.nvertices; ++i)
    {
        for (int j = i + 1; j < graph.nvertices; ++j)
        {
            if (matrix[i][j] != 0)
            {
                DrawLineWithArrow(painter, QPoint(graph.vertexCoord[i].x, graph.vertexCoord[i].y), QPoint(graph.vertexCoord[j].x, graph.vertexCoord[j].y));
            }
            if (matrix[j][i] != 0)
            {
                DrawLineWithArrow(painter, QPoint(graph.vertexCoord[j].x, graph.vertexCoord[j].y), QPoint(graph.vertexCoord[i].x, graph.vertexCoord[i].y));
            }
        }
        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.drawEllipse(graph.vertexCoord[i].x - graph.r / 2, graph.vertexCoord[i].y - graph.r / 2, graph.r, graph.r);
        painter.setFont(QFont("times", 22));
        painter.drawText(graph.vertexCoord[i].x - graph.r / 5, graph.vertexCoord[i].y + graph.r / 5, QString::number(i+1));
    }
}

void Dialog::DrawLineWithArrow(QPainter& painter, QPoint start, QPoint end)
{
  qreal arrowSize = 20;
  painter.setBrush(Qt::black);

  QLineF line(end, start);
  double angle = std::atan2(-line.dy(), line.dx());
  QPoint vector = end - start;
  double length = sqrt(vector.x() * vector.x() + vector.y() * vector.y());
  double s1 = graph.r/2*((double)vector.x()) / length;
  double s2 = graph.r/2*((double)vector.y()) / length;

  QPoint sd(s1, s2);
  QPoint endpoint = end - sd;
  QPointF arrowP1 = endpoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                        cos(angle + M_PI / 3) * arrowSize);
  QPointF arrowP2 = endpoint + QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize,
                                        cos(angle + M_PI - M_PI / 3) * arrowSize);

  QPolygonF arrowHead;
  arrowHead.clear();
  arrowHead << endpoint << arrowP1 << arrowP2;
  painter.drawLine(endpoint, start);
  painter.drawPolygon(arrowHead);
}

void Dialog::on_pushButton_draw_vertices_clicked()
{
    int nvertices = ui->lineEdit_number_of_vertices->text().toInt();
    if(nvertices != graph.nvertices)
    {
        if (nvertices > MAXV)
        {
            QMessageBox::critical(this, "Ошибка", "Введенное число вершин превышает допустимое значение!");
            return;
        }
        graph.nvertices = nvertices;
        graph.GetCoordsArray();
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
    for (int i = 0; i < graph.nvertices; ++i)
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
        graph.nedges++;
        netflow->insert_flow_edge(&graph, vertex1+1, vertex2+1, true, weight);



        QString leText = ui->plainTextEdit_reslut->toPlainText();
        leText.append("Edge: (%1, %2), weight:%3");
        leText = leText.arg(vertex1+1).arg(vertex2+1).arg(weight);
        leText.append("\n");
        ui->plainTextEdit_reslut->setPlainText(leText);
        repaint();
    }
}

void Dialog::on_pushButton_FindFlow_clicked()
{
    int source = 1, target = graph.nvertices/2 +1;
    netflow->netflow(&graph, source, target);
    print_result();
}

void Dialog::print_result()
{
    int i;
    EdgeNode *p;
    QString result;


    for (i=1; i<=graph.nvertices; i++)
    {
        p = graph.edges[i];
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

    int flow = 0;
    p = graph.edges[1];
    while (p != NULL)
    {
        flow += p->flow;
        p = p->next;
    }
    result.append("\n");
    result.append("Total flow = %1\n");
    result = result.arg(flow);
    ui->plainTextEdit_reslut->setPlainText(result);
}
