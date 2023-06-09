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
    if (client1->connectDevice()) {
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
    if (client2->connectDevice()) {
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
    connect(wSocket, &QWebSocket::textMessageReceived, tcw, &TCW241::lecturesite);
    chrono2->start(2000); //toutes les 2 secondes, on affiche les donnees sur le site
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

void Serveur::trameTemperature()
{
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
    QModbusDataUnit donnees = reponsecapt4->result();
    if (donnees.value(0) == 1)
        donneesJson.insert("NiveauEau", "Suffisant");
    else
        donneesJson.insert("NiveauEau", "Insuffisant");
    reponsecapt4->deleteLater();
    reponserelay = client2->sendReadRequest(relayEau, 3);
    if (!reponserelay->isFinished()) {
        connect(reponserelay, SIGNAL(finished()), this, SLOT(chercheEau()));
    }
}

void Serveur::chercheEau()
{
    QModbusDataUnit donnees = reponserelay->result();
    if (donnees.value(0) == 1)
        eau = true; //eau de pluie utilisee
    else
        eau = false; //eau du reseau utilisee
    reponsecapt5 = client2->sendReadRequest(donneesDebit, 3);
    if (!reponsecapt5->isFinished()) {
        connect(reponsecapt5, SIGNAL(finished()), this, SLOT(trameDebit()));
    }
}

void Serveur::trameDebit()
{
    QModbusDataUnit donnees = reponsecapt5->result();
    QJsonValue val = QString::number(donnees.value(0), 10) + " L";
    if (eau == true)
    {
        donneesJson.insert("ConsoEauPluie", val);
        donneesJson.insert("ConsoEauCourante", QString::number(0));
    }
    else if (eau == false)
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
