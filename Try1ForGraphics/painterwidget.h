#ifndef PAINTERWIDGET_H
#define PAINTERWIDGET_H

#include <QWidget>
#include "datahandler.h"
#include <QTimer>
class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);
    ~PainterWidget();
    DataHandler* dh;

protected:
    void paintEvent(QPaintEvent *event);
public slots:
    void UpdateData();
private:
    int timer_interval = 1500;
    QTimer* timer;
};
#endif // PAINTERWIDGET_H
