#include "graphicswindow.h"
#include "ui_graphicswindow.h"
#include "painterwidget.h"
#include <QSignalMapper>
GraphicsWindow::GraphicsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GraphicsWindow)
{
    ui->setupUi(this);
    graphicsPainter = new PainterWidget(ui->graphicsView);

    this->setWindowTitle ("Task Manager @ Danon");
    QRect screen_size = QApplication::desktop ()->screenGeometry (this);

    ui->graphicsView->setMinimumSize (screen_size.width () * 0.5,screen_size.height () * 0.5);
    ui->graphicsView->setMaximumSize (screen_size.width (),screen_size.height ());

    ui->plainTextEdit->setHidden (true);
    ui->plainTextEdit->setMinimumSize (screen_size.width () * 0.5,screen_size.height () * 0.5);
    ui->plainTextEdit->setMaximumSize (screen_size.width (),screen_size.height ());

    SetTraySystem ();
    SetMenuNavigation ();

}

void GraphicsWindow::SetMenuNavigation (){
    QMenu *buttonMenu = new QMenu(this);
    QString action_names[7] {
        "RAM",
        "SWAP",
        "InTraffic",
        "OutTraffic",
        "CPU",
        "Cores",
        "Info"
    };
    for (int i = 0 ; i < 7; i++){
        QAction *action = new QAction(action_names[i],this);
        QSignalMapper *mapper = new QSignalMapper();
        connect(action,SIGNAL(triggered(bool)),mapper,SLOT(map()));
        mapper->setMapping (action,i);
        connect (mapper,SIGNAL(mapped(int)),this,SLOT(GraphButtonPressed(int)));
        buttonMenu->addAction(action);
    }

    buttonMenu->setTitle (QString("Menu"));
    ui->menuBar->addMenu (buttonMenu);
}

void GraphicsWindow::SetTraySystem (){
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


void GraphicsWindow::GraphButtonPressed (int index){
    if (index == _CORES){
        CoresButtonPressed ();
        return;
    }
    else if (index > _CORES){
        TextButtonPressed ();
        return;
    }
    ui->verticalSlider->setEnabled (true);
    ui->plainTextEdit->setHidden (true);
    ui->graphicsView->setHidden (false);
    resizeEvent (nullptr);
    graphicsPainter->isInTray = false;
    graphicsPainter->dh->SetCurrentDimension (index);
    update ();
}
void GraphicsWindow::CoresButtonPressed (){
    ui->verticalSlider->setEnabled (false);
    ui->plainTextEdit->setHidden (true);
    ui->graphicsView->setHidden (false);
    resizeEvent (nullptr);
    graphicsPainter->isInTray = false;
    graphicsPainter->dh->SetCurrentDimension (_CORES);
    update ();
}
void GraphicsWindow::TextButtonPressed (){
    ui->plainTextEdit->setHidden (false);
    ui->graphicsView->setHidden (true);
    ui->verticalSlider->setEnabled (false);
    graphicsPainter->isInTray = true;
    resizeEvent (nullptr);
    update ();
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
void GraphicsWindow::on_verticalSlider_valueChanged(int value)
{
    graphicsPainter->dh->ExpandDataSet (value);
    graphicsPainter->update ();
}
