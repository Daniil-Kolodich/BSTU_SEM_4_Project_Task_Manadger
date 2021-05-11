#ifndef GRAPHICSWINDOW_H
#define GRAPHICSWINDOW_H
#include "painterwidget.h"
#include "enumForData.h"
#include <QMenu>
#include <QStyle>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class GraphicsWindow; }
QT_END_NAMESPACE

class GraphicsWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphicsWindow(QWidget *parent = nullptr);
    ~GraphicsWindow();

    void resizeEvent (QResizeEvent* event);
    void closeEvent (QCloseEvent *event);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_verticalSlider_valueChanged(int value);

    void iconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    Ui::GraphicsWindow *ui;
    PainterWidget* graphicsPainter;
    QSystemTrayIcon *trayIcon;
};
#endif // GRAPHICSWINDOW_H
