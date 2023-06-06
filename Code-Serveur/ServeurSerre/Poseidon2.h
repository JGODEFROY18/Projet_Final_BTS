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
    void trameLevel();
    void trameDebit();
    bool eau;//eau de pluie->true et eau courante->false
};
