#include "ProjetSerre.h"

ProjetSerre::ProjetSerre(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    socket = new QTcpSocket(this);
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    //QObject::connect(socket, SIGNAL(connected()), this, SLOT(receptionTrame()));
}

ProjetSerre::~ProjetSerre()
{}

bool niveau = false;

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


void ProjetSerre::onGetLevelClicked()
{
    //char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x02, 0x00, 0x63, 0x00, 0x04 }; pour lire l'entrée 1 à 4
    // entrée 1 uniquement char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x02, 0x00, 0x63, 0x00, 0x01 };
    
    //Trame de lecture pour l'entrée 1 d'adresse 199 (63 en hexa 
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x02, 0x00, 0x63, 0x00, 0x01 };
    QByteArray trameEnvoi(trame, 12);
    socket->write(trameEnvoi);
    QByteArray data = socket->readAll();
    //Obtenir un pointeur vers les données sous-jacentes
    char* rawData = data.data();
    int dataSize = data.size();
    char lastByte = rawData[dataSize - 1];
    //ui.labelNiveau->setText(QString::number(lastByte, 16));
    int lastByteInt = static_cast<int>(lastByte);
    if (lastByteInt == 1) {
        niveau = true;
    ui.labelNiveau->setText("Niveau : suffisant");
    }
    else if(lastByteInt == 0) {
        ui.labelNiveau->setText("Niveau : insuffisant");
    }
}

void ProjetSerre::onDisplayDebitClicked()
{
    //Décalage au niveau des adresses IN1=201, IN2=203, IN4=207 ici débit est sur IN4
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x04, 0x00, 0xCF, 0x00, 0x01 };
    QByteArray trameEnvoi(trame, 12);
    socket->write(trameEnvoi);
    QByteArray data = socket->readAll();
    //Obtenir un pointeur vers les données sous-jacentes
    char* rawData = data.data();
    int dataSize = data.size();
    QByteArray lastTwoBytes(rawData + dataSize - 2, 2);
    int decimalValue = QString::fromLatin1(lastTwoBytes.toHex()).toInt(nullptr, 16);
    ui.labelDebit->setText(QString::number(decimalValue));

    // remettre le compteur à 0 
    /*
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x11, 0x10, 0x00, 0xCF, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00 };
    QByteArray trameEnvoi(trame, 17);
    socket->write(trameEnvoi);
    */
}

// di na bind daal: 11 02 00 C4 00 16 BAA9 heure bi na gaaw diot way li yagg na ba eupp 

void ProjetSerre::onDisplayTemperatureClicked()
{

}

void ProjetSerre::onRelayOnClicked()
{
    //eau courante - on NO1
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC7, 0xFF, 0x00 };

    QByteArray trameRelay1(trame, 12);

    socket->write(trameRelay1);
}

void ProjetSerre::onRelayOffClicked()
{
    //eau de pluie - off NC1
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC7, 0x00, 0x00 };

    QByteArray trameRelay1(trame, 12);

    socket->write(trameRelay1);
}

void ProjetSerre::activatePompe()
{
    if (niveau == true){
    // fonction 5 (correspond au type dans la doc)  on : FF00 et off : 0000
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC8, 0xFF, 0x00 };

    QByteArray trameRelay2(trame, 12);

    socket->write(trameRelay2);
    }
}

void ProjetSerre::EteindrePompe()
{
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC8, 0x00, 0x00 };

    QByteArray trameRelay2(trame, 12);

    socket->write(trameRelay2);
}






























  