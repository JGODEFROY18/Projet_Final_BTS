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
    // Cr�er une requ�te Modbus pour lire la temp�rature � l'adresse 101-100
    QByteArray request;
    QDataStream stream(&request, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << quint16(0x0000); // Transaction ID (0x0000 pour Modbus/TCP)
    stream << quint16(0x0000); // Protocole ID (0x0000 pour Modbus/TCP)
    stream << quint16(0x0006); // Longueur de la requ�te en octets
    stream << quint8(0x00); // Unit� d'adresse (0x00 pour Modbus/TCP)
    stream << quint8(0x03); // Code fonction (0x03 pour lecture de plusieurs registres)
    stream << quint16(0x0064); // Adresse de d�part � lire (101-100 = 0x0064)
    stream << quint16(0x0002); // Nombre de registres � lire (2)
    QByteArray crc = QByteArray::number(qChecksum(request.data(), request.size()), 16).rightJustified(4, '0').toUpper();
    stream << quint16(crc.toUShort(nullptr, 16)); // CRC de la requ�te

   
        // Envoyer la requ�te au serveur distant
        socket->write(request);
    socket->flush();

    // Attendre la r�ponse du serveur
    if (!socket->waitForReadyRead(5000)) {
        ui.labelStatus->setText("Statut: Erreur de lecture");
        return;
    }

    // Lire la r�ponse du serveur
    QByteArray response = socket->readAll();
    QDataStream responseStream(&response, QIODevice::ReadOnly);
    responseStream.setByteOrder(QDataStream::BigEndian);
    quint16 transactionId;
    quint16 protocolId;
    quint16 length;
    quint8 unitId;
    quint8 functionCode;
    quint8 byteCount;
    quint16 register1;
    quint16 register2;
    responseStream >> transactionId >> protocolId >> length >> unitId >> functionCode >> byteCount >> register1 >> register2;

    // Afficher la temp�rature lue � l'�tiquette d'affichage correspondante
    QString temperature = QString::number((register1 << 16) | register2);
    ui.labelTemperature->setText("Temperature: " + temperature + " �C");



   /* // Envoyer une demande de lecture de la temp�rature � l'adresse 101-100
    QByteArray request;
    request.append(0x00); // ID du p�riph�rique
    request.append(0x03); // Code de fonction pour la lecture de donn�es
    request.append(0x01); // Adresse haute (101)
    request.append(0x00); // Adresse basse (100)
    request.append(0x00); // Nombre d'octets � lire (2)
    socket->write(request);

    // Attendre la r�ponse du serveur (2 octets pour la temp�rature)
    if (socket->waitForReadyRead()) {
        QByteArray response = socket->readAll();
        if (response.size() == 5 && response[0] == 0x00 && response[1] == 0x03) {
            // Extraire la temp�rature � partir de la r�ponse (les 2 octets suivants)
            int temperature = (response[3] << 8) | response[4];

            // Afficher la temp�rature dans l'interface utilisateur
            ui.labelTemperature->setText(QString("Temp�rature : %1").arg(temperature));
        }
    }*/

    /*// Envoyer une demande de lecture des donn�es de temp�rature au port 1
    QByteArray request;
    QDataStream out(&request, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::BigEndian);
    out << quint8(0x01) << quint8(0x04) << quint16(0x0000) << quint16(0x0001);

    socket->write(request);

    // Attendre la r�ponse du serveur
    if (socket->waitForReadyRead()) {
        QByteArray response = socket->readAll();

        // Traiter les donn�es de temp�rature
        quint16 temperature = (quint8(response.at(3)) << 8) | quint8(response.at(4));
        QString temperatureStr = QString::number(temperature / 10.0, 'f', 1);
        ui.labelTemperature->setText("Temperature: " + temperatureStr + " �C");
    }*/
}




