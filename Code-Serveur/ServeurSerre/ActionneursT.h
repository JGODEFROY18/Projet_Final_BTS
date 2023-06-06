#pragma once
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QModbusRequest>

class ActionneursT : public QObject
{
    Q_OBJECT

public:
    ActionneursT(QModbusTcpClient*);
    ~ActionneursT();

public slots:

private:
    
};

