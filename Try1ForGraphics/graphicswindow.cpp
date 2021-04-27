#include "graphicswindow.h"
#include "ui_graphicswindow.h"
#include "painterwidget.h"
#include <QDebug>
#include <QResizeEvent>
GraphicsWindow::GraphicsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphicsWindow)
{
    ui->setupUi(this);
    graphicsPainter = new PainterWidget(ui->graphicsView);

    // random values, adjust later to some ratio with user screen size
    ui->graphicsView->setMinimumSize (300,300);
    ui->graphicsView->setMaximumSize (1500,2000);
}

void GraphicsWindow::resizeEvent (QResizeEvent *event){
    Q_UNUSED(event)
    // setting new area for painting
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
    if (ui->pushButton->text () == QString("CPU")){
        graphicsPainter->dh->SetCurrentDimension (0);
        ui->pushButton->setText (QString("CoreMode"));

        ui->verticalSlider->setEnabled (true);
    }
    else {
        ui->verticalSlider->setEnabled (false);
        graphicsPainter->dh->SetCurrentDimension (1);
        ui->pushButton->setText (QString("CPU"));
    }

    // for screen update NOW
    graphicsPainter->update ();
}

void GraphicsWindow::on_pushButton_2_clicked()
{
    // setting to draw RAM

    ui->verticalSlider->setEnabled (true);
    ui->pushButton->setText (QString("CPU"));
    graphicsPainter->dh->SetCurrentDimension (-1);

    // for screen update NOW
    graphicsPainter->update ();
}


void GraphicsWindow::on_pushButton_3_clicked()
{
    // setting to draw Wi-Fi
    if (ui->pushButton_3->text () == QString("Incom")){
        graphicsPainter->dh->SetCurrentDimension (-2);
        ui->pushButton_3->setText (QString("Outcom"));
    }
    else {
        graphicsPainter->dh->SetCurrentDimension (-3);
        ui->pushButton_3->setText (QString("Incom"));
    }


    // for screen update NOW
    graphicsPainter->update ();
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

    // for screen update NOW
    graphicsPainter->update ();
}
