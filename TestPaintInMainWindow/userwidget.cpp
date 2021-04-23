#include "userwidget.h"
#include <QPainter>
#include <time.h>
#include <QDebug>
UserWidget::UserWidget(QWidget *parent) : QWidget(parent)
{
    data = new int[10];
    srand(time(NULL));
    for (int i = 0 ; i  < 10 ; i++){
        data[i] = rand() % 100;
    }
}

void UserWidget::paintEvent (QPaintEvent *event){
    QPainter painter(this);
    qDebug() << width();
    painter.drawEllipse (50,50,50,50);
    QPoint points[10];
    for (int i = 0 ; i < 10; i++){
        points[i].setX (i * width() / 10);
        points[i].setY (height () / 100 * data[i]);

    }
    painter.drawPolyline (points,10);
}
