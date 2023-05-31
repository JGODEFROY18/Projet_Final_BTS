#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QTimer>


class WebSocket : public QObject
{
	Q_OBJECT

public:
	WebSocket(QObject* parent = Q_NULLPTR);
	QWebSocket* webSocket = nullptr;

public slots:
	void webSocketConnected();
	//void envoi();

private:
	QWebSocketServer* webSocketServer;
	QTimer* chrono;
	//QJsonObject jsonObject;
};
