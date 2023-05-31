#include "WebSocket.h"

WebSocket::WebSocket(QObject* parent)
{
	chrono = new QTimer();
	webSocketServer = new QWebSocketServer(QStringLiteral("Server WebSocket"), QWebSocketServer::NonSecureMode);

	if (this->webSocketServer->listen(QHostAddress::AnyIPv4, 12345))
	{
		qDebug() << "Server WebSocket started" << "\n";
		QObject::connect(webSocketServer, SIGNAL(newConnection()), this, SLOT(webSocketConnected()));
	}
	else
	{
		qDebug() << "Server WebSocket: Erreur d'ecoute sur le port 12345" << "\n";
	}
}

void WebSocket::webSocketConnected()
{
	webSocket = this->webSocketServer->nextPendingConnection();
	qDebug() << "Nouvelle connexion sur le port 12345";
}