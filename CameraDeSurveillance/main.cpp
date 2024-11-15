#include "CameraDeSurveillance.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CameraDeSurveillance w;
    w.show();
    return a.exec();
}
