#include "graphicswindow.h"
#include "ui_graphicswindow.h"
#include "painterwidget.h"

GraphicsWindow::GraphicsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphicsWindow)
{
    ui->setupUi(this);
    graphicsPainter = new PainterWidget(ui->graphicsView);

    this->setWindowTitle ("Task Manager @ Danon");
    QRect screen_size = QApplication::desktop ()->screenGeometry (this);
//    ui->centralwidget->setMinimumSize (screen_size.width () * 0.7,screen_size.height () * 0.7);
//    ui->centralwidget->setMaximumSize (screen_size.width (),screen_size.height ());
    ui->graphicsView->setMinimumSize (screen_size.width () * 0.5,screen_size.height () * 0.5);
    ui->graphicsView->setMaximumSize (screen_size.width (),screen_size.height ());
    ui->plainTextEdit->setHidden (true);
    ui->plainTextEdit->setMinimumSize (screen_size.width () * 0.5,screen_size.height () * 0.5);
    ui->plainTextEdit->setMaximumSize (screen_size.width (),screen_size.height ());
//    ui->graphicsView->setBaseSize (1500,1000);
//    ui->graphicsView->set
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon (this->style ()->standardIcon (QStyle::SP_ComputerIcon));
    trayIcon->setToolTip ("Task Manager @ Danon");
    QMenu *menu = new QMenu(this);
    QAction *viewAction = new QAction(QString ("Open"),this);
    QAction *quitAction = new QAction(QString ("Exit"),this);
    connect (viewAction,SIGNAL(triggered()),this,SLOT(show()));
    connect (quitAction,SIGNAL(triggered()),this,SLOT(close()));
    menu->addAction(viewAction);
    menu->addAction(quitAction);
    trayIcon->setContextMenu (menu);
    trayIcon->show ();
    connect (trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));


}

void GraphicsWindow::resizeEvent (QResizeEvent *event){
    Q_UNUSED(event)
    graphicsPainter->setGeometry (0,0,ui->graphicsView->width (),ui->graphicsView->height ());
}

GraphicsWindow::~GraphicsWindow()
{
    graphicsPainter->deleteLater ();
    delete ui;
}


void GraphicsWindow::on_pushButton_clicked()
{
    // setting to draw CPU

    ui->pushButton_4->setText ("Info");
    ui->plainTextEdit->setHidden (true);
    ui->graphicsView->setHidden (false);
    graphicsPainter->isInTray = false;
    resizeEvent (nullptr);
    if (ui->pushButton->text () == QString("CPU")){
        graphicsPainter->dh->SetCurrentDimension (_CPU);
        ui->pushButton->setText (QString("Cores"));

        ui->verticalSlider->setEnabled (true);
    }
    else {
        ui->verticalSlider->setEnabled (false);
        graphicsPainter->dh->SetCurrentDimension (_CORES);
        ui->pushButton->setText (QString("CPU"));
    }

    // for screen update NOW
    graphicsPainter->update ();
}

void GraphicsWindow::on_pushButton_2_clicked()
{
    // setting to draw RAM

    ui->pushButton_4->setText ("Info");
    ui->plainTextEdit->setHidden (true);
    ui->graphicsView->setHidden (false);
    graphicsPainter->isInTray = false;
    resizeEvent (nullptr);
    ui->verticalSlider->setEnabled (true);
    if (ui->pushButton_2->text () == QString("RAM")){
        graphicsPainter->dh->SetCurrentDimension (_RAM);
        ui->pushButton_2->setText (QString("SWAP"));
    }
    else {
        graphicsPainter->dh->SetCurrentDimension (_SWAP);
        ui->pushButton_2->setText (QString("RAM"));
    }
    // for screen update NOW
    graphicsPainter->update ();
}


void GraphicsWindow::on_pushButton_3_clicked()
{
    // setting to draw Wi-Fi
    ui->pushButton_4->setText ("Info");
    ui->plainTextEdit->setHidden (true);
    ui->graphicsView->setHidden (false);
    graphicsPainter->isInTray = false;
    resizeEvent (nullptr);
    ui->verticalSlider->setEnabled (true);
    if (ui->pushButton_3->text () == QString("InTraffic")){
        graphicsPainter->dh->SetCurrentDimension (_IN_TRAFFIC);
        ui->pushButton_3->setText (QString("OutTraffic"));
    }
    else {
        graphicsPainter->dh->SetCurrentDimension (_OUT_TRAFFIC);
        ui->pushButton_3->setText (QString("InTraffic"));
    }


    // for screen update NOW
    graphicsPainter->update ();
}

void GraphicsWindow::iconActivated (QSystemTrayIcon::ActivationReason reason){
   if (reason == QSystemTrayIcon::Trigger){
       if (!this->isVisible ()){
           this->show ();
           graphicsPainter->isInTray = false;
       }
       else{
           this->hide ();
           graphicsPainter->isInTray = true;
       }
   }

}

void GraphicsWindow::closeEvent (QCloseEvent *event){
    Q_UNUSED(event)
    exit(0);
}
void GraphicsWindow::on_pushButton_4_clicked()
{
    ui->graphicsView->setHidden (true);
    ui->plainTextEdit->setHidden (false);
    graphicsPainter->isInTray = true;
    ui->verticalSlider->setEnabled (false);
    resizeEvent (nullptr);
//    ui->plainTextEdit->setPlainText ("LOADING...");
    ui->plainTextEdit->setPlainText (graphicsPainter->dh->GetInfoText());
    if (ui->pushButton_4->text () == "Info")
        ui->pushButton_4->setText ("Refresh");

}
void GraphicsWindow::on_verticalSlider_valueChanged(int value)
{
    graphicsPainter->dh->ExpandDataSet (value);
    graphicsPainter->update ();
}
