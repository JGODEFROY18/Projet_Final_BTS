#include "Serveur.h"

Serveur::Serveur()
{
    params = new QSettings("config.ini", QSettings::IniFormat);
    client1 = new QModbusTcpClient(this);
    client2 = new QModbusTcpClient(this);
    createClient1();
    createClient2();
    donneesBacs = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 17500, 6);
    donneesSensor = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 19800, 4);
    donneesTemp = QModbusDataUnit(QModbusDataUnit::InputRegisters, 100, 1);
    donneesLevel = QModbusDataUnit(QModbusDataUnit::DiscreteInputs, 99, 1);
    donneesDebit = QModbusDataUnit(QModbusDataUnit::InputRegisters, 207, 1);
    relayEau = QModbusDataUnit(QModbusDataUnit::Coils, 200, 1);
    tcw = new TCW241(client1);
    pos = new Poseidon2(params);
    chrono1 = new QTimer();
    chrono2 = new QTimer();
    //
    //conexion Websocket
    //
    wSocketServer = new QWebSocketServer(QStringLiteral("Server WebSocket"), QWebSocketServer::NonSecureMode);

    if (this->wSocketServer->listen(QHostAddress::AnyIPv4, params->value("Websocket/port").toInt())) {
        QObject::connect(wSocketServer, SIGNAL(newConnection()), this, SLOT(wSocketConnected()));
        qDebug() << "Server WebSocket: Debut d'ecoute sur le port" << params->value("Websocket/port").toInt();
    }
    else {
        qDebug() << "Server WebSocket: Erreur d'ecoute sur le port" << params->value("Websocket/port").toInt();
    }
    //
    //Envoi en BDD
    //
    QObject::connect(chrono1, SIGNAL(timeout()), this, SLOT(envoiBDD()));
    chrono1->start(600000); //toutes les 10 minutes, on stocke les donnees en BDD
}

Serveur::~Serveur()
{
}

void Serveur::createClient1()
{
    client1->setConnectionParameter(QModbusDevice::NetworkAddressParameter, params->value("TCW241/ip").toString());
    client1->setConnectionParameter(QModbusDevice::NetworkPortParameter, params->value("TCW241/port").toInt());
    client1->connectDevice();
    if (client1->state() == QModbusDevice::ConnectedState) {
        qDebug() << "connection tcw reussie \nip :" << params->value("TCW241/ip").toString() << "\nport :" << params->value("TCW241/port").toInt();
    }
    else {
        qDebug("connection ratee");
    }
}

void Serveur::createClient2()
{
    client2->setConnectionParameter(QModbusDevice::NetworkAddressParameter, params->value("Poseidon2/ip").toString());
    client2->setConnectionParameter(QModbusDevice::NetworkPortParameter, params->value("Poseidon2/port").toInt());
    client2->connectDevice();
    if (client2->state() == QModbusDevice::ConnectedState) {
        qDebug() << "connection poseidon reussie \nip : " << params->value("Poseidon2/ip").toString() << "\nport : " << params->value("Poseidon2/port").toInt();
    }
    else {
        qDebug("Where connexion");
    }
}

void Serveur::envoiBDD()
{
    Donnees();
    modifJSon();
    QNetworkRequest request(QUrl(params->value("API/url").toString()));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QJsonDocument doc(donneesJson);
    QByteArray data = doc.toJson();
    QString donnee = data;
    QUrlQuery requete;
    requete.addQueryItem("donnees", donnee);
    QNetworkReply* reply = mgr->post(request, requete.query().toUtf8());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QString contents = QString::fromUtf8(reply->readAll());
        }
        else {
            QString err = reply->errorString();
            qDebug() << err;
        }
        reply->deleteLater();
        }
    );
}

void Serveur::modifJSon()
{
    int taille = donneesJson.size();
    QStringList cles = donneesJson.keys();
    for (int z = 0; z < taille; z++) {
        if ((cles[z].contains("Temp", Qt::CaseSensitive))) {
            QString peintre = donneesJson.value(cles[z]).toString();
            peintre.chop(2);
            donneesJson[cles[z]] = peintre;
        }
        else if ((cles[z].contains("Humid", Qt::CaseSensitive))) {
            QString peintre = donneesJson.value(cles[z]).toString();
            peintre.chop(1);
            donneesJson[cles[z]] = peintre;
        }
    }
}

void Serveur::wSocketConnected()
{
    wSocket = this->wSocketServer->nextPendingConnection();
    wSocket->sendTextMessage("Bonjour Client");
    qDebug("Nouveau client connecte");
    QObject::connect(chrono2, SIGNAL(timeout()), this, SLOT(lecturedonnees()));
    connect(wSocket, &QWebSocket::textMessageReceived, this, &Serveur::lecturesite);
    chrono2->start(2000); //toutes les 2 secondes, on affiche les donnees sur le site
}

void Serveur::lecturesite(QString message){
    qDebug()<<message;
    int lec = message.toInt();
    switch(lec){
    case 0:
        //ouverture fenetre
        tcw->Relay4ON();
        break;
    case 1:
        //fermeture fenetre
        tcw->Relay4OFF();
        break;
    case 2:
        //ouverture brumisation
        tcw->Relay1ON();
        //envoi de la requete de la temperature exterieure
        reponsetrame1 = client2->sendReadRequest(donneesTemp, 2);
        if (!reponsetrame1->isFinished()) {
            connect(reponsetrame1, SIGNAL(finished()), this, SLOT(ActionTemperature()));
        }
        pos->reseauEau();
        break;
    case 3:
        //fermeture brumisation
        tcw->Relay1OFF();
        pos->stopPump();
        pos->eauPluie();
        break;
    case 4:
        //ouverture chauffage
        tcw->Relay3ON();
        break;
    case 5:
        //fermeture chauffage
        tcw->Relay3OFF();
        break;
    case 6:
        //ouverture arrosage
        tcw->Relay2ON();
        //envoi de la requete de la temperature exterieure
        reponsetrame1 = client2->sendReadRequest(donneesTemp, 2);
        if (!reponsetrame1->isFinished()) {
            connect(reponsetrame1, SIGNAL(finished()), this, SLOT(ActionTemperature()));
        }
        pos->reseauEau();
        break;
    case 7:
        //fermeture arrosage
        tcw->Relay2OFF();
        pos->stopPump();
        pos->eauPluie();
        break;
    default:
        qDebug("Fonction non disponible");
    }
}

void Serveur::wSocketDeconnected()
{
    qDebug("Client deconnecte");
    QObject::disconnect(chrono2, SIGNAL(timeout()), this, SLOT(lecturedonnees()));
    chrono2->stop();
}

void Serveur::lecturedonnees()
{
    Donnees();
    QJsonDocument doc(donneesJson);
    QByteArray docsend = doc.toJson();
    QString data(docsend);
    if (wSocket != nullptr) wSocket->sendTextMessage(data);
}

void Serveur::Donnees()
{
    reponsecapt1 = client1->sendReadRequest(donneesSensor, 1);
    if (!reponsecapt1->isFinished()) {
        connect(reponsecapt1, SIGNAL(finished()), this, SLOT(DataSensor()));
    }
}

void Serveur::DataSensor()
{
    QModbusDataUnit donnees = reponsecapt1->result();
    quint16 temp[2] = { donnees.value(0),donnees.value(1) };
    quint16 humid[2] = { donnees.value(2),donnees.value(3) };
    donneesJson.insert("TempInt", tcw->valeurJson(temp, 't'));
    donneesJson.insert("HumidInt", tcw->valeurJson(humid, 'h'));
    reponsecapt1->deleteLater();
    reponsecapt2 = client1->sendReadRequest(donneesBacs, 1);
    if (!reponsecapt2->isFinished()) {
        connect(reponsecapt2, SIGNAL(finished()), this, SLOT(DataCapteurs()));
    }
}

void Serveur::DataCapteurs()
{
    QModbusDataUnit donnees = reponsecapt2->result();
    quint16 humids1[2] = { donnees.value(0),donnees.value(1) };
    quint16 humids2[2] = { donnees.value(2),donnees.value(3) };
    quint16 humids3[2] = { donnees.value(4),donnees.value(5) };
    donneesJson.insert("HumidSol1", tcw->valeurJson(humids1, 'H'));
    donneesJson.insert("HumidSol2", tcw->valeurJson(humids2, 'H'));
    donneesJson.insert("HumidSol3", tcw->valeurJson(humids3, 'H'));
    reponsecapt2->deleteLater();
    reponsecapt3 = client2->sendReadRequest(donneesTemp, 2);
    if (!reponsecapt3->isFinished()) {
        connect(reponsecapt3, SIGNAL(finished()), this, SLOT(trameTemperature()));
    }
}

void Serveur::ActionTemperature()
{
    //reception de la requete de la temperature exterieure
    QModbusDataUnit donnees = reponsetrame1->result();
    quint16 temp = donnees.value(0);
    float arr = temp / 10.0;
    pos->setTemperature(arr);
    reponsetrame1->deleteLater();
    //envoi de la requete du niveau d'eau
    reponsetrame2 = client2->sendReadRequest(donneesLevel, 2);
    if (!reponsetrame2->isFinished()) {
        connect(reponsetrame2, SIGNAL(finished()), this, SLOT(ActionLevel()));
    }
}

void Serveur::ActionLevel()
{
    //reception de la requete du niveau d'eau
    QModbusDataUnit donnees = reponsetrame2->result();
    if (donnees.value(0) == 1)
        pos->setLevel(true);
    else
        pos->setLevel(false);
    reponsetrame2->deleteLater();
}

void Serveur::trameTemperature()
{
    //temperature exterieure
    QModbusDataUnit donnees = reponsecapt3->result();
    quint16 temp = donnees.value(0);
    float arr = temp / 10.0;
    QString stemp = QString::number(double(arr), 'f', 1) + 0xB0 + "C";
    donneesJson.insert("TempExt", stemp);
    reponsecapt3->deleteLater();
    reponsecapt4 = client2->sendReadRequest(donneesLevel, 2);
    if (!reponsecapt4->isFinished()) {
        connect(reponsecapt4, SIGNAL(finished()), this, SLOT(trameLevel()));
    }
}

void Serveur::trameLevel()
{
    //niveau d'eau
    QModbusDataUnit donnees = reponsecapt4->result();
    if (donnees.value(0) == 1)
        donneesJson.insert("NiveauEau", "Suffisant");
    else
        donneesJson.insert("NiveauEau", "Insuffisant");
    reponsecapt4->deleteLater();
    reponserelay = client2->sendReadRequest(relayEau, 3);
    if (!reponserelay->isFinished()) {
        connect(reponserelay, SIGNAL(finished()), this, SLOT(CapteurEau()));
    }
}

void Serveur::CapteurEau()
{
    QModbusDataUnit donnees = reponserelay->result();
    if (donnees.value(0) == 1)
        eauPluie = true; //eau de pluie utilisee
    else
        eauPluie = false; //eau du reseau utilisee
    reponserelay->deleteLater();
    reponsecapt5 = client2->sendReadRequest(donneesDebit, 3);
    if (!reponsecapt5->isFinished()) {
        connect(reponsecapt5, SIGNAL(finished()), this, SLOT(trameDebit()));
    }
}

void Serveur::trameDebit()
{
    //debit d'eau
    QModbusDataUnit donnees = reponsecapt5->result();
    QJsonValue val = QString::number(donnees.value(0), 10) + " L";
    if (eauPluie == true)
    {
        donneesJson.insert("ConsoEauPluie", val);
        donneesJson.insert("ConsoEauCourante", QString::number(0));
    }
    else if (eauPluie == false)
    {
        donneesJson.insert("ConsoEauPluie", QString::number(0));
        donneesJson.insert("ConsoEauCourante", val);
    }
    else
    {
        donneesJson.insert("ConsoEauPluie", QString::number(0));
        donneesJson.insert("ConsoEauCourante", QString::number(0));
    }
    reponsecapt5->deleteLater();
}
