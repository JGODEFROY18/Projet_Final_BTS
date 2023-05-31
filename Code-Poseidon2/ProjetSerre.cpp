#include "ProjetSerre.h"

//définition des Transaction_ID pour chaque trame de lecture (2e octet de la trame)
#define TEMPERATURE_ID 0x02 
#define DEBIT_ID 0x01
#define LEVEL_ID 0x03

ProjetSerre :: ProjetSerre(QObject* parent)
{
	socket = new QTcpSocket(this);
	chrono = new QTimer();
	srand(time(NULL));
	socket->connectToHost(ip, port);
	QObject::connect(socket, &QTcpSocket::connected, this, &ProjetSerre::socketConnected);
	QObject::connect(socket, &QTcpSocket::disconnected, this, &ProjetSerre::socketDisconnected);

	chrono->start(500);

	QObject::connect(chrono, SIGNAL(timeout()), this, SLOT(envoiWebSocket()));

	state = 0;
	
	QObject::connect(socket, &QTcpSocket::readyRead, this, &ProjetSerre::handleCardSentence);
}

ProjetSerre::~ProjetSerre()
{
}

void ProjetSerre::socketConnected()
{
	qDebug() << "Socket status : Connected";
}

void ProjetSerre::socketDisconnected()
{
	qDebug() << "Socket status : disconnected";
}

//envoi de la trame de lecture 
void ProjetSerre::trameTemperature()
{
	char trameTemp[] = { 0x00, TEMPERATURE_ID, 0x00, 0x00, 0x00, 0x06, 0x11, 0x04, 0x00, 0x64, 0x00, 0x01 };
	QByteArray trameEnvoiTemp(trameTemp, 12);
	socket->write(trameEnvoiTemp);
	
}

void ProjetSerre::trameLevel()
{
	char trameNiv[] = { 0x00, LEVEL_ID, 0x00, 0x00, 0x00, 0x06, 0x11, 0x02, 0x00, 0x63, 0x00, 0x01 };
	QByteArray trameEnvoiNiv(trameNiv, 12);
	socket->write(trameEnvoiNiv);
}


void ProjetSerre::trameDebit()
{
	char trameDeb[] = { 0x00, DEBIT_ID, 0x00, 0x00, 0x00, 0x06, 0x11, 0x04, 0x00, 0xCF, 0x00, 0x01 };
	QByteArray trameEnvoiDeb(trameDeb, 12);
	socket->write(trameEnvoiDeb);
}

void ProjetSerre::resetCounterDebit()
{
	char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x11, 0x10, 0x00, 0xCF, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00 };
	QByteArray trameEnvoi(trame, 17);
	socket->write(trameEnvoi);
}

//méthode pour la réception des données des différents capteurs
void ProjetSerre::handleCardSentence()
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
		jsonObject.insert("TempExt", Temperature);
	}
	// Débit :
	else if (rawData[1] == DEBIT_ID)
	{
		QByteArray lastTwoBytesDeb(rawData + dataSize - 2, 2);
		int decimalValueDeb = QString::fromLatin1(lastTwoBytesDeb.toHex()).toInt(nullptr, 16);
		if (eau == true) {
		jsonObject["consoEauPluie"] = decimalValueDeb;
		jsonObject["consoEauCourante"] = 0;
		qDebug() << "Conso Eau de pluie :" << decimalValueDeb;
		}
		else if(eau == false)
		{
			jsonObject["consoEauCourante"] = decimalValueDeb;
			jsonObject["consoEauPluie"] = 0;
			qDebug() << "Conso Eau courante :" << decimalValueDeb;
		}
		else
		{
			jsonObject["consoEauCourante"] = 0;
			jsonObject["consoEauPluie"] = 0;
			qDebug() << "Conso d'eau :" << decimalValueDeb;
		}
	}
	// Niveau :
	else if (rawData[1] == LEVEL_ID)
	{
		char lastByteNiv = rawData[dataSize - 1];
		int lastByteInt = static_cast<int>(lastByteNiv);
		if (lastByteInt == 1) {
			jsonObject["NiveauEau"] = "Suffisant";
			qDebug() << "Niveau eau Suffisant";
			this->level = true;
		}
		else if (lastByteInt == 0) {
			jsonObject["NiveauEau"] = "Insuffisant";
			qDebug() << "Niveau eau Insuffisant";
			this->level = false;
		}
	}
}


void ProjetSerre::envoiWebSocket()
{
	//compteur state pour que tous les capteurs soient lus avant d'envoyer et d'afficher les données 
	if(state == 0)
		trameDebit();
	else if(state == 1)
		trameTemperature();
	else if(state == 2)
		trameLevel();  
	else
	{
		QJsonDocument jsonDocument(jsonObject);
		QByteArray docsend = jsonDocument.toJson();
		QString data(docsend);
		QWebSocket* websocket = WebSocket.webSocket;
		if (websocket != nullptr) websocket->sendTextMessage(data);
		state = -1; //pour qu'il passe à 1 après
	}

	state++;
}

void ProjetSerre::activatePump()
{
		char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC8, 0xFF, 0x00 };

		QByteArray trameRelay2(trame, 12);

		socket->write(trameRelay2);
		qDebug() << "La pompe est allumee\n";
}
void ProjetSerre::stopPump()
{
	char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC8, 0x00, 0x00 };

	QByteArray trameRelay2(trame, 12);

	socket->write(trameRelay2);
	qDebug() << "La pompe est eteinte\n";
}
		
void ProjetSerre::eauPluie()
{
		char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC7, 0x00, 0x00 };

		QByteArray trameRelay1(trame, 12);

		socket->write(trameRelay1);
		eau = true;
}

void ProjetSerre::eauCourante()
{
	//eau courante - on NO1
	
		char trame[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x05, 0x00, 0xC7, 0xFF, 0x00 };

		QByteArray trameRelay1(trame, 12);

		socket->write(trameRelay1);
		eau = false;
}

void ProjetSerre::reseauEau()
{
	if (level == true && temperature > 3)
	{
		activatePump();
		resetCounterDebit();
		eauPluie();
		qDebug() << "Activation du réseau d'eau de pluie\n";
	}
	else if (level != true || temperature < 3)
	{
		resetCounterDebit();
		eauCourante();
		qDebug() << "Activation du réseau d'eau courante\n";
	}
}



