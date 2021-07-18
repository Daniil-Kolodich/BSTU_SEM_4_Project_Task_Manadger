#include "graphicswindow.h"

#include <QApplication>
#include <locale.h>


int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"ru_RU.UTF-8");
    QApplication a(argc, argv);
    GraphicsWindow w;
    w.show();
    return a.exec();
}
