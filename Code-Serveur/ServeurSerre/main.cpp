#include <QtCore/QCoreApplication>
#include "Serveur.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Serveur serv;
    return a.exec();
}
