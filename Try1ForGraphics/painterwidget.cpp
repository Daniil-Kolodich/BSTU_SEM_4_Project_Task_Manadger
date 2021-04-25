#include "painterwidget.h"
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
PainterWidget::PainterWidget(QWidget *parent) : QWidget(parent)
{
    // perhaps i should try send amount of cores + 2 for second param
    // & actually should i be able to set this fuckind data, dont think so
    dh = new DataHandler();
}

void PainterWidget::paintEvent (QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    // IDK seems beauty
    int x_offset = 50;
    int y_offset = 10;
    // just decorations
    painter.drawRect (x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset);
    // kinda main part for drawing graphs
    dh->DataDrawer (&painter,QRect(x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset));
    // get new data
    dh->GetNewData ();
    // update actually calls paintEvent
    update ();
}
PainterWidget::~PainterWidget()
{
    delete dh;
}

