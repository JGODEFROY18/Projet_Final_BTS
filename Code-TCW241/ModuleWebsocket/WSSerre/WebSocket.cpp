#include "WebSocket.h"

WebSocket::WebSocket(QObject* parent)
{
	chrono = new QTimer();
	srand(time(NULL));
	wSocketServer = new QWebSocketServer(QStringLiteral("Server WebSocket"), QWebSocketServer::NonSecureMode);
	chrono->start(2000);
	QObject::connect(chrono, SIGNAL(timeout()), this, SLOT(envoi()));
	if (this->wSocketServer->listen(QHostAddress::AnyIPv4, 12345))
	{
		qDebug() << "Server WebSocket: Nouvelle connexion sur le port 12345" << "\n";
		QObject::connect(wSocketServer, SIGNAL(newConnection()), this, SLOT(wSocketConnected()));
	}
	else
	{
		qDebug() << "Server WebSocket: Erreur d'ecoute sur le port 12345" << "\n";
	}
}

void WebSocket::wSocketConnected()
{
	wSocket = this->wSocketServer->nextPendingConnection();
	qDebug() << "Nouvelle connection";
}

void WebSocket::createJson()
{
    donneesJson.insert("ConsoElec", rand() % 100);
    donneesJson.insert("ConsoEau", rand() % 100);
    donneesJson.insert("TempInt", rand() % 100);
    donneesJson.insert("HumidInt", rand() % 100);
    donneesJson.insert("TempExt", rand() % 100);
    donneesJson.insert("HumidSol1", rand() % 100);
    donneesJson.insert("HumidSol2", rand() % 100);
    donneesJson.insert("HumidSol3", rand() % 100);
}

void WebSocket::envoi()
{
	createJson();
	QJsonDocument doc(donneesJson);
	QByteArray docsend = doc.toJson();
	QString data(docsend);
	if (wSocket != nullptr) wSocket->sendTextMessage(data);
}
