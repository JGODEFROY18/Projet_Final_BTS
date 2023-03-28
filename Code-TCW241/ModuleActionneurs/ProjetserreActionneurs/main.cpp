#include "Actionneurs.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Actionneurs w;
    w.show();
    return a.exec();
}
