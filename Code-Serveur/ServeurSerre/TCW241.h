#ifndef TCW241_H
#define TCW241_H
#pragma once
#include <QtCore/QCoreApplication>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QSettings>
#include <math.h>

class TCW241 : public QObject
{
    Q_OBJECT

public:
    TCW241(QModbusTcpClient*);
    ~TCW241();
    QJsonValue valeurJson(quint16*, char);
    //fonctions pour allumer ou eteindre l'arrosoir
    void Relay1ON(); void Relay1OFF();
    //fonctions pour allumer ou eteindre la brumisation
    void Relay2ON(); void Relay2OFF();
    //fonctions pour allumer ou eteindre le chauffage
    void Relay3ON(); void Relay3OFF();
    //fonctions pour ouvrir ou fermer la fenetre
    void Relay4ON(); void Relay4OFF();

public slots:
    void actiondel();

private:
    QJsonObject donneesJson;
    QModbusTcpClient* client;
    //variables contenant le type de registre, l'adresse Modbus et le nombre d'octet ou de bits a obtenir/modifier.
    QModbusDataUnit Relay1, Relay2, Relay3, Relay4;
    QModbusReply* reponseact = nullptr;
    float calculHumidite(float);
};

#endif // TCW241_H
