#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    int* data;
};

/*
class SecondUserWidget : private UserWidget{
    Q_OBJECT
public:
    explicit SecondUserWidget(QWidget *parent = 0) : UserWidget(parent){

    }

protected:
    void paintEvent(QPaintEvent *event) override{
        // using qDebug() << for debugging
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
};
*/
#endif // USERWIDGET_H
