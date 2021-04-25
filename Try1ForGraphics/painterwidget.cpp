#include "painterwidget.h"
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QDebug>
PainterWidget::PainterWidget(QWidget *parent) : QWidget(parent)
{
    // perhaps i should try send amount of cores + 2 for second param
    // & actually should i be able to set this fuckind data, dont think so
    dh = new DataHandler();
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateData()));
}
void PainterWidget::UpdateData (){
    dh->SecondPartOfGettingData ();
    update ();
}
void PainterWidget::paintEvent (QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint (QPainter::Antialiasing);
    // IDK seems beauty
    int x_offset = 10;
    int y_offset = 5;
    // just decorations
    painter.drawRect (x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset);
    // kinda main part for drawing graphs
    dh->DataDrawer (&painter,QRect(x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset));
    // get new data
    dh->FirstPartOfGettingData ();
    timer->setInterval (timer_interval);
    timer->start ();
}
PainterWidget::~PainterWidget()
{
    delete dh;
}

