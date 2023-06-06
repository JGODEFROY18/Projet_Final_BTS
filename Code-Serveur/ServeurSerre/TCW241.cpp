#include "tcw241.h"

TCW241::TCW241(QModbusTcpClient* cli)
{
    client = cli;
    Relay1 = QModbusDataUnit(QModbusDataUnit::Coils, 100, 1);
    Relay2 = QModbusDataUnit(QModbusDataUnit::Coils, 101, 1);
    Relay3 = QModbusDataUnit(QModbusDataUnit::Coils, 102, 1);
    Relay4 = QModbusDataUnit(QModbusDataUnit::Coils, 103, 1);
}

TCW241::~TCW241()
{
    delete client;
}

void TCW241::lecturesite(QString message) {
    switch (message.toInt()) {
    case 0:
        Relay4ON();
        break;
    case 1:
        Relay4OFF();
        break;
    case 2:
        Relay1ON();
        break;
    case 3:
        Relay1OFF();
        break;
    case 4:
        Relay3ON();
        break;
    case 5:
        Relay3OFF();
        break;
    case 6:
        Relay2ON();
        break;
    case 7:
        Relay2OFF();
        break;
    default:
        qDebug("Fonction non disponible");
    }
}

QJsonValue TCW241::valeurJson(quint16* arr, char car)
{
    static_assert(std::numeric_limits<float>::is_iec559, "Only supports IEC 559 (IEEE 754) float");

    //La variable temp est un entier de 32 bits, arr est un tableau de 2 cases d'entiers de 16 bits.
    //En partant de la fin, le programme place arr[0] au bit 16 puis arr[1] au bit 0
    quint32 temp = (arr[0] << 16) | arr[1];

    //le compilateur va forcer la transformation d'un quint32 en float
    float* val = reinterpret_cast<float*>(&temp);
    QString affichage;
    switch (car) {
    case 't':
        affichage = QString::number(double(*val), 'f', 1) + 0xB0 + "C";
        break;
    case 'H':
        *val = calculHumidite(*val);
    case 'h':
        affichage = QString::number(double(*val), 'f', 1) + "%";
        break;
    default:
        affichage = QString::number(double(*val), 'f', 1);
        return 0;
    }
    QJsonValue valeur(affichage);
    return valeur;
}

float TCW241::calculHumidite(float Vout)
{
    float humid = -1.91 * pow(10, -9) * pow(Vout, 3);
    humid += 1.33 * pow(10, -5) * pow(Vout, 2);
    humid += 9.56 * pow(10, -3) * Vout;
    humid -= 2.16 * pow(10, 1);
    return humid;
}

void TCW241::actiondel()
{
    reponseact->deleteLater();
}

void TCW241::Relay1ON()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0064 FF00"));
    reponseact = client->sendRawRequest(request, 100);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay1OFF()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0064 0000"));
    reponseact = client->sendRawRequest(request, 100);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay2ON()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0065 FF00"));
    reponseact = client->sendRawRequest(request, 101);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay2OFF()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0065 0000"));
    reponseact = client->sendRawRequest(request, 101);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay3ON()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0066 FF00"));
    reponseact = client->sendRawRequest(request, 102);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay3OFF()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0066 0000"));
    reponseact = client->sendRawRequest(request, 102);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay4ON()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0067 FF00"));
    reponseact = client->sendRawRequest(request, 103);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}

void TCW241::Relay4OFF()
{
    QModbusRequest request(QModbusRequest::WriteSingleCoil, QByteArray::fromHex("0067 0000"));
    reponseact = client->sendRawRequest(request, 103);
    if (!reponseact->isFinished()) {
        connect(reponseact, SIGNAL(finished()), this, SLOT(actiondel()));
    }
}
