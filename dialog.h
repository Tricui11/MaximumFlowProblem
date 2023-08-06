#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QRegularExpressionValidator>
#include <flowgraph.cpp>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();


    void DrawLineWithArrow(QPainter& painter, QPoint start, QPoint end);



protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_pushButton_draw_vertices_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_FindFlow_clicked();

private:
    Ui::Dialog *ui;
    QValidator *cellValidator;

    void print_result();
};
#endif // DIALOG_H
