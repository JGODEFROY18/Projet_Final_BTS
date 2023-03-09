#include "ProjetSerre.h"

ProjetSerre::ProjetSerre(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    socket = new QTcpSocket(this);
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}

ProjetSerre::~ProjetSerre()
{}

void ProjetSerre::onConnectButtonClicked()
{
    socket->connectToHost("192.168.65.10", 502);
}

void ProjetSerre::onSocketConnected()
{
    ui.labelConnexion->setText("Etat : Connected");
}

void ProjetSerre::onSocketDisconnected()
{
    ui.labelConnexion->setText("Etat : Disconnected");
}

void ProjetSerre::onSocketReadyRead()
{
    ui.labelStatus->setText("Statut: Donnees recues");
}

void ProjetSerre::onDisplayTemperatureClicked()
{
   
}




