#include "ProjetSerre.h"

ProjetSerre::ProjetSerre(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    socket = new QTcpSocket(this);
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(receptionTrame()));
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
    if (socket->state() == QAbstractSocket::ConnectedState) {

        // Définition d'une trame Modbus TCP de 12 octets à envoyer au serveur
        char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x02, 0x04, 0x00, 0x64, 0x00, 0x01 };

        // Création d'un tableau de bytes contenant la trame
        QByteArray data(trame, 12);

        // Envoi de la trame au serveur
        socket->write(data);

        // Vide le tampon de sortie de la socket
        socket->flush();
    }
}


void ProjetSerre::receptionTrame() 
{
    QByteArray data = socket->readAll();
    data = data.right(2);
    static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    quint32 temp = ((quint8)data[0] << 24) | ((quint8)data[1] << 16) | 0 ;

    //La variable temp est un entier de 32 bits, arr est un tableau de 4 cases d'entiers de 8 bits.
    //En partant de la fin, le programme place arr[0] au bit 24 puis arr[0] au bit 16 puis arr[0] au bit 8 et arr[0] au bit 0

    float* out = reinterpret_cast<float*>(&temp);
    //le compilateur va forcer la transformation d'un quint32 en float
    
    QString temperature;
    temperature.setNum(*out);
    
    ui.labelTemperature->setText(temperature);
    //return *out;
}



