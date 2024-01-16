#include "GraphicSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphicSys w;
    w.show();
    return a.exec();
}
