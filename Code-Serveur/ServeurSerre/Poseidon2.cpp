#include "Poseidon2.h"
//définition des Transaction_ID pour chaque trame de lecture (2e octet de la trame)
#define DEBIT_ID 0x01
#define TEMPERATURE_ID 0x02 
#define LEVEL_ID 0x03

Poseidon2::Poseidon2(QSettings* params)
{
	socket = new QTcpSocket(this);
	socket->connectToHost(params->value("Poseidon2/ip").toString(), params->value("Poseidon2/port").toInt());
	state = 0;
	QObject::connect(socket, &QTcpSocket::readyRead, this, &Poseidon2::handleCardSentence);
	eau = false;
	level = false;
	temperature = 0.0f;
}

Poseidon2::~Poseidon2()
{
}

void Poseidon2::resetCounterDebit()
{
    char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x11, 0x10, 0x00, static_cast<char>(0xCF), 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00 };
    QByteArray trameEnvoi(trame, 17);
    socket->write(trameEnvoi);
}
//envoi de la trame de lecture 
void Poseidon2::trameTemperature()
{
	char trameTemp[] = { 0x00, TEMPERATURE_ID, 0x00, 0x00, 0x00, 0x06, 0x11, 0x04, 0x00, 0x64, 0x00, 0x01 };
	QByteArray trameEnvoiTemp(trameTemp, 12);
	socket->write(trameEnvoiTemp);

}

void Poseidon2::trameLevel()
{
	char trameNiv[] = { 0x00, LEVEL_ID, 0x00, 0x00, 0x00, 0x06, 0x11, 0x02, 0x00, 0x63, 0x00, 0x01 };
	QByteArray trameEnvoiNiv(trameNiv, 12);
	socket->write(trameEnvoiNiv);
}

void Poseidon2::trameDebit()
{
	char trameDeb[] = { 0x00, DEBIT_ID, 0x00, 0x00, 0x00, 0x06, 0x11, 0x04, 0x00, static_cast<char>(0xCF), 0x00, 0x01 };
	QByteArray trameEnvoiDeb(trameDeb, 12);
	socket->write(trameEnvoiDeb);
}

//méthode pour la réception des données des différents capteurs
void Poseidon2::handleCardSentence()
{
	QByteArray dataTemp = socket->readAll();
	//Obtenir un pointeur vers les données sous-jacentes
	char* rawData = dataTemp.data();
	int dataSize = dataTemp.size();
	// Température :
	if (rawData[1] == TEMPERATURE_ID)
	{
		QByteArray lastTwoBytesTemp(rawData + dataSize - 2, 2);
		float decimalValueTemp = QString::fromLatin1(lastTwoBytesTemp.toHex()).toInt(nullptr, 16);
		float Temperature = decimalValueTemp / 10;
		qDebug() << "Temperature exterieure :" << Temperature;
	}
	// Débit :
	else if (rawData[1] == DEBIT_ID)
	{
		QByteArray lastTwoBytesDeb(rawData + dataSize - 2, 2);
		int decimalValueDeb = QString::fromLatin1(lastTwoBytesDeb.toHex()).toInt(nullptr, 16);
		if (eau == true) {
			qDebug() << "Conso Eau de pluie :" << decimalValueDeb;
		}
		else if (eau == false)
		{
			qDebug() << "Conso Eau courante :" << decimalValueDeb;
		}
		else
		{
			qDebug() << "Conso d'eau :" << decimalValueDeb;
		}
	}
	// Niveau :
	else if (rawData[1] == LEVEL_ID)
	{
		char lastByteNiv = rawData[dataSize - 1];
		int lastByteInt = static_cast<int>(lastByteNiv);
		if (lastByteInt == 1) {
			this->level = true;
		}
		else if (lastByteInt == 0) {
			this->level = false;
		}
	}
}

void Poseidon2::activatePump()
{
    char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, static_cast<char>(0xC8), static_cast<char>(0xFF), 0x00 };

    QByteArray trameRelay2(trame, 12);

    socket->write(trameRelay2);
    qDebug() << "La pompe est allumee\n";
}

void Poseidon2::stopPump()
{
    char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, static_cast<char>(0xC8), 0x00, 0x00 };

    QByteArray trameRelay2(trame, 12);

    socket->write(trameRelay2);
    qDebug() << "La pompe est eteinte\n";
}

void Poseidon2::eauPluie()
{
    char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, static_cast<char>(0xC7), 0x00, 0x00 };

    QByteArray trameRelay1(trame, 12);

    socket->write(trameRelay1);
    eau = true;
}

void Poseidon2::eauCourante()
{
    //eau courante - on NO1

    char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, static_cast<char>(0xC7), static_cast<char>(0xFF), 0x00 };

    QByteArray trameRelay1(trame, 12);

    socket->write(trameRelay1);
    eau = false;
}

void Poseidon2::reseauEau()
{
    if (level == true && temperature > 3)
    {
        activatePump();
        resetCounterDebit();
        eauPluie();
        qDebug() << "Activation du reseau d'eau de pluie";
    }
    else if (level != true || temperature < 3)
    {
		stopPump();
        resetCounterDebit();
        eauCourante();
        qDebug() << "Activation du reseau d'eau courante";
    }
}
