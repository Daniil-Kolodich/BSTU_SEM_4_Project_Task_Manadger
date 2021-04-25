#ifndef PAINTERWIDGET_H
#define PAINTERWIDGET_H

#include <QWidget>
#include "datahandler.h"
class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);
    ~PainterWidget();
    DataHandler* dh;

protected:
    void paintEvent(QPaintEvent *event);

};
#endif // PAINTERWIDGET_H
