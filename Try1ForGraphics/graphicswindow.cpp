#include "graphicswindow.h"
#include "ui_graphicswindow.h"
#include "painterwidget.h"
#include <QDebug>
GraphicsWindow::GraphicsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphicsWindow)
{
    ui->setupUi(this);
    // making our painterWidget & set drawable area to ui->GraphicsView
    graphicsPainter = new PainterWidget(ui->graphicsView);
    // setting geometry IDK without this not working at all
    graphicsPainter->setGeometry (0,0,ui->graphicsView->width (),ui->graphicsView->height ());
}

GraphicsWindow::~GraphicsWindow()
{
    // IDK found somewhere
    graphicsPainter->deleteLater ();
    delete ui;
}


void GraphicsWindow::on_pushButton_clicked()
{
    // setting to draw CPU
    graphicsPainter->dh->SetCurrentDimension (0);
}

void GraphicsWindow::on_pushButton_2_clicked()
{
    // setting to draw RAM
    graphicsPainter->dh->SetCurrentDimension (-1);
}

void GraphicsWindow::on_verticalSlider_valueChanged(int value)
{
    // changing range of data
    graphicsPainter->dh->ExpandDataSet (value);
}
