#include <QtCore/QCoreApplication>
#include "donnees.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    donnees donnees;
    return a.exec();
}
