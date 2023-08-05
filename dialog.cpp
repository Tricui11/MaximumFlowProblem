#include "dialog.h"
#include "ui_dialog.h"
#include "netflow.cpp"


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_pushButton_clicked()
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
}

