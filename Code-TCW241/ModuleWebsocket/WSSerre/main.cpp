#include <QtCore/QCoreApplication>
#include "WebSocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebSocket w;

    return a.exec();
}
