#include "ProjetSerre.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ProjetSerre w;
    w.show();
    return a.exec();
}
