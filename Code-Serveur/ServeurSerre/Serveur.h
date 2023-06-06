#pragma once
#include "tcw241.h"
#include "poseidon2.h"
#include <QtCore/QCoreApplication>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QUrlQuery>
#include <QWebSocket>
#include <QWebSocketServer>

class Serveur : public QObject
{
    Q_OBJECT

public:
    Serveur();
    ~Serveur();

public slots:
    void wSocketConnected();
    void wSocketDeconnected();
    void envoiBDD();
    void lecturedonnees();
    void Donnees();
    void DataSensor();
    void DataCapteurs();
    void trameTemperature();
    void trameLevel();
    void chercheEau();
    void trameDebit();

private:
    void createClient1();
    void createClient2();
    void modifJSon();
    bool eau;//eau de pluie->true et eau courante->false
    TCW241* tcw;
    Poseidon2* pos;
    QJsonObject donneesJson;
    QWebSocket* wSocket = nullptr;
    QWebSocketServer* wSocketServer;
    //Parametres present dans le fichier config.ini
    QSettings* params;
    //chrono pour la BDD
    QTimer* chrono1;
    //chrono pour le WebSocket
    QTimer* chrono2;
    //variable contenant l'ip et le port des cartes E/S
    QModbusTcpClient* client1, * client2;
    //Variable utilisee pour l'envoi en BDD
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    //variable contenant le type de registre, l'adresse Modbus et le nombre d'octet ou de bits a obtenir/modifier.
    QModbusDataUnit donneesBacs, donneesSensor, donneesTemp, donneesLevel, donneesDebit, relayEau;
    //variable contenant les reponses avec les donnees des capteurs
    QModbusReply* reponsecapt1 = nullptr, * reponsecapt2 = nullptr, * reponsecapt3 = nullptr, * reponsecapt4 = nullptr, * reponsecapt5 = nullptr;
    QModbusReply* reponserelay = nullptr;
};
