#include <dialog.h>
#include <ui_dialog.h>
#include <QMessageBox>
#include <netflow.cpp>
#include <QPainter>
#include <QRegularExpression>

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

int matrix[12][12];
int temp[12][12];
Graph graph10(10);

QString symbol[12] ={"A", "B","C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};

Graph Dialog::getGraph()
{
    return graph10;
}

void Dialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this); // Создаём объект отрисовщика
    // Устанавливаем кисть абриса
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    Graph graph = getGraph();
    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
    painter.setFont(QFont("times", 22));
    for (int i = 0; i < graph.capacity; ++i)
    {
        for (int j = i + 1; j < graph.capacity; ++j)
            if (matrix[i][j] != 0)
                painter.drawLine(graph.vertexCoord[i].x, graph.vertexCoord[i].y, graph.vertexCoord[j].x, graph.vertexCoord[j].y);
        painter.drawEllipse(graph.vertexCoord[i].x - graph.r / 2, graph.vertexCoord[i].y - graph.r / 2, graph.r, graph.r);
        painter.drawText (graph.vertexCoord[i].x - graph.r / 5, graph.vertexCoord[i].y + graph.r / 5, symbol[i]);
    }
}

void Dialog::on_pushButton_draw_vertices_clicked()
{
    repaint();
}

void Dialog::on_pushButton_add_clicked()
{
    Graph graph = getGraph();
    QString str_vertex1 = ui->lineEdit_vertex1->text();
    QString str_vertex2 = ui->lineEdit_vertex_2->text();
    QString str_weight = ui->lineEdit_weight->text();
    int weight = str_weight.toInt();
    int vertex1 = -1;
    int vertex2 = -1;
    for (int i = 0; i < graph.capacity; ++i)
    {
        if (str_vertex1 == symbol[i])
        {
            vertex1 = i;
        }
        if (str_vertex2 == symbol[i])
        {
            vertex2 = i;
        }
    }
    if (vertex1 == -1 || vertex2 == -1 || vertex1 == vertex2)
        QMessageBox::critical(this, "Ошибка", "Неверно введены вершины");
    else
    {
        matrix[vertex1][vertex2] = weight;
        matrix[vertex2][vertex1] = weight;
        repaint();
    }
}

void Dialog::on_pushButton_FindFlow_clicked()
{

    FlowGraph g;
    int source, sink;
    int flow;
    EdgeNode *p;


    source = 1;
    sink = 8;

    Netflow *netflow = new Netflow();
    netflow->read_flow_graph(&g, true);

    netflow->netflow(&g, source, sink);

    netflow->print_flow_graph(&g);

    flow = 0;
    p = g.edges[source];
    while (p != NULL)
    {
        flow += p->flow;
        p = p->next;
    }

    printf("total flow = %d\n",flow);





    Graph graph = getGraph();
    int checkLines = 0;
    for (int i = 0; i < graph.capacity; ++i){
        for (int j = 0; j < graph.capacity; ++j) {
            if (matrix[i][j] != 0) {
                ++checkLines;
                break;
            }
        }
    }
    if (checkLines != graph.capacity) QMessageBox::critical(this, "Ошибка", "Граф должен быть связным");
}
