#include "Capteurs.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Capteurs w;
    w.show();
    return a.exec();
}
