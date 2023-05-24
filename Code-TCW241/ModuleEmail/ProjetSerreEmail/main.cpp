#include <QtCore/QCoreApplication>
#include "Email.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Email email("smtp.gmail.com", "serre.lapro@gmail.com", ""); //need JL pour MDP
    return a.exec();
}
