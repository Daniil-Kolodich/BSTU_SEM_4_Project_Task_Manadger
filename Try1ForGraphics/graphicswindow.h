#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H

#include <QMainWindow>
#include "painterwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class GraphicsWindow; }
QT_END_NAMESPACE

class GraphicsWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphicsWindow(QWidget *parent = nullptr);
    ~GraphicsWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_verticalSlider_valueChanged(int value);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::GraphicsWindow *ui;
    PainterWidget* graphicsPainter;
};
#endif // GRAPHICSWINDOW_H
