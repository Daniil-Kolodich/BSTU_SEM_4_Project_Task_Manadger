#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userwidget.h"
#include <QPainter>
#include <QWidget>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // adding field for drawing where
    widg = new UserWidget(ui->graphicsView);
    // neccessary for setting size of drawing field
    widg->setGeometry (0,0,ui->graphicsView->width (),ui->graphicsView->height ());
}

MainWindow::~MainWindow()
{
    widg->deleteLater ();
    delete ui;
}

