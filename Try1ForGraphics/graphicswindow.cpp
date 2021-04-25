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
    graphicsPainter->setGeometry (ui->graphicsView->x (),ui->graphicsView->y (),ui->graphicsView->width (),ui->graphicsView->height ());
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
    if (ui->pushButton->text () == QString("CPU")){
        graphicsPainter->dh->SetCurrentDimension (0);
        ui->pushButton->setText (QString("CoreMode"));
        ui->verticalSlider->setHidden (false);
    }
    else {
        ui->verticalSlider->setHidden (true);
        graphicsPainter->dh->SetCurrentDimension (1);
        ui->pushButton->setText (QString("CPU"));
    }
}

void GraphicsWindow::on_pushButton_2_clicked()
{
    // setting to draw RAM

    ui->verticalSlider->setHidden (false);
    ui->pushButton->setText (QString("CPU"));
    graphicsPainter->dh->SetCurrentDimension (-1);
}


void GraphicsWindow::on_pushButton_3_clicked()
{
    // setting to draw Wi-Fi

    ui->verticalSlider->setHidden (false);
    ui->pushButton->setText (QString("CPU"));
    graphicsPainter->dh->SetCurrentDimension (-2);
}

void GraphicsWindow::on_pushButton_4_clicked()
{
    // setting to switch Mode for CPU drawing
//    graphicsPainter->dh->SetCurrentDimension (-10);
//    ui->textBrowser->setHidden (!ui->textBrowser->isHidden ());
}
void GraphicsWindow::on_verticalSlider_valueChanged(int value)
{
    // changing range of data
    graphicsPainter->dh->ExpandDataSet (value);
}
