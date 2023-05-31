#pragma once
#include <QtCore/QCoreApplication>
#include <QTcpSocket>
#include <QTimer>
#include "WebSocket.h"

class ProjetSerre : public QObject
{
	Q_OBJECT

public:
	ProjetSerre(QObject* parent = Q_NULLPTR);
	~ProjetSerre();


public slots:
	void socketConnected();
	void socketDisconnected();

	void trameTemperature();
	void trameLevel();
	void trameDebit();
	void resetCounterDebit(); 
	void handleCardSentence();
	void envoiWebSocket();

	void activatePump();
	void stopPump();
	void eauPluie();
	void eauCourante(); 

	void reseauEau();

private:
	QTcpSocket* socket;
	QString ip = "192.168.65.10";
	quint16 port = 502;
	QTimer* chrono;
	WebSocket WebSocket;
	QJsonObject jsonObject;

	bool level;
	float temperature;
	bool eau; //eau de pluie->true et eau courante->false

	int state; //pour l'envoi des trames de lecture des capteurs 
};