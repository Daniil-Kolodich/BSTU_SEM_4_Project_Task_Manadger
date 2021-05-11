#include "painterwidget.h"

PainterWidget::PainterWidget(QWidget *parent) : QWidget(parent)
{
    dh = new DataHandler();
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(UpdateData()));

    dh->FirstPartOfDataUpdate ();
    timer->setInterval (timer_interval);
    timer->start ();
}


void PainterWidget::UpdateData (){
    dh->SecondPartOfDataUpdate ();
    dh->UpdateData ();
    if (!isInTray)
        update ();
    dh->FirstPartOfDataUpdate ();
    timer->setInterval (timer_interval);
    timer->start ();
}
void PainterWidget::paintEvent (QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint (QPainter::Antialiasing);
    int x_offset = 10;
    int y_offset = 10;
    painter.drawRect (QRectF(x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset));
    dh->DataDrawer (&painter,QRectF(x_offset,y_offset,width () - 2 * x_offset, height () - 2 * y_offset));
}

PainterWidget::~PainterWidget()
{
    delete dh;
}

