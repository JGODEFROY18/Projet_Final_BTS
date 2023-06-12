#ifndef POSEIDON2_H
#define POSEIDON2_H
#pragma once
#include <QtCore/QCoreApplication>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QSettings>
#include <QTcpSocket>
#include <QTimer>

class Poseidon2 : public QObject
{
    Q_OBJECT

public:
    Poseidon2(QSettings*);
    ~Poseidon2();
    void trameLevel();
    void trameDebit();
    void setTemperature(float temp){
        this->temperature = temp;
    }
    void setLevel(bool niveau){
        this->level = niveau;
    }

public slots:

    void resetCounterDebit();
    void handleCardSentence();
    void trameTemperature();
    void activatePump();
    void stopPump();
    void eauPluie();
    void eauCourante();
    void reseauEau();


private:
    QTcpSocket* socket;
    bool level;
    float temperature;
    int state;
    bool eau;//eau de pluie->true et eau courante->false
};

#endif // POSEIDON2_H
