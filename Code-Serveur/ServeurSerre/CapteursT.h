#pragma once
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <math.h>

class CapteursT : public QObject
{
    Q_OBJECT

public:
    CapteursT(QModbusTcpClient*);
    ~CapteursT();

private:
};

