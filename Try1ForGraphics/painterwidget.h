#ifndef PAINTERWIDGET_H
#define PAINTERWIDGET_H

#include "datahandler.h"
#include "enumForData.h"
#include <QWidget>
#include <QTimer>
class PainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PainterWidget(QWidget *parent = nullptr);
    ~PainterWidget();
    DataHandler* dh;
    bool isInTray = false;
public slots:
    void UpdateData();
private:
    void paintEvent(QPaintEvent *event);
    int timer_interval = 1000;
    QTimer* timer;
};
#endif // PAINTERWIDGET_H
