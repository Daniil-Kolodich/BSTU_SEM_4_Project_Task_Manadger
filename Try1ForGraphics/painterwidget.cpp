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
    dh = new DataHandler();
    timer = new QTimer();
    // connecting timer::timeout to updatingData
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateData()));

    // preset for updating data
    dh->FirstPartOfDataUpdate ();
    timer->setInterval (timer_interval);
    timer->start ();
}


void PainterWidget::UpdateData (){
    /*
    once this started
    it will recall itself until end
    kinda usefull cause
    that's gimme separate drawing & updating data
    */
    dh->SecondPartOfDataUpdate ();
    dh->UpdateData ();
    update ();
    // preset
    // timer
    dh->FirstPartOfDataUpdate ();
    timer->setInterval (timer_interval);
    timer->start ();
}
void PainterWidget::paintEvent (QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    // found somewhere, look better
    painter.setRenderHint (QPainter::Antialiasing);
    // IDK seems beauty
    int x_offset = 10;
    int y_offset = 5;
    // just decorations
    painter.drawRect (x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset);
    // kinda main part for drawing graphs
    dh->DataDrawer (&painter,QRect(x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset));
}

PainterWidget::~PainterWidget()
{
    delete dh;
}

