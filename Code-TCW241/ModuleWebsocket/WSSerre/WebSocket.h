#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>
#include <cstdlib>
#include <ctime>

class WebSocket : public QObject
{
	Q_OBJECT

public:
	WebSocket(QObject* parent = Q_NULLPTR);

public slots:
	void wSocketConnected();
	void envoi();

private:
	void createJson();
	QJsonObject donneesJson;
	QJsonValue valeur;
	QTimer* chrono;
	QWebSocketServer* wSocketServer;
	QWebSocket* wSocket = nullptr;

};

