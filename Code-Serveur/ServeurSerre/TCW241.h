#pragma once
#include <QtCore/QCoreApplication>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QSettings>

class TCW241 : public QObject
{
    Q_OBJECT

public:
    TCW241(QSettings*);
    ~TCW241();
    QJsonValue valeurJson(quint16*, char);

public slots:
    void lecturesite(QString);
    void actiondel();
    //fonctions pour allumer ou eteindre l'arrosoir
    void Relay1ON(); void Relay1OFF();
    //fonctions pour allumer ou eteindre la brumisation
    void Relay2ON(); void Relay2OFF();
    //fonctions pour allumer ou eteindre le chauffage
    void Relay3ON(); void Relay3OFF();
    //fonctions pour ouvrir ou fermer la fenetre
    void Relay4ON(); void Relay4OFF();

private:
    QJsonObject donneesJson;
    QModbusTcpClient* client;
    //variables contenant le type de registre, l'adresse Modbus et le nombre d'octet ou de bits à obtenir/modifier.
    QModbusDataUnit Relay1, Relay2, Relay3, Relay4;
    QModbusReply* reponseact = nullptr;
    float calculHumidite(float);
};