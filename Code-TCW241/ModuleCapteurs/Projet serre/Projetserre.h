#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Projetserre.h"
#include "CalculJson.h"
#include <QTcpSocket>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QWebSocketServer>
#include <QWebSocket>

class Projetserre : public QMainWindow
{
    Q_OBJECT

public:
    Projetserre(QWidget *parent = nullptr);
    ~Projetserre();

public slots:
	void onConnectButtonClicked();
	void onDisconnectButtonClicked();
	void onSocketConnected();
	void onSocketDisconnected();
	void wSocketConnected();
	void wSocketDisconnected();
	void DonneesSensor();
	void DonneesCapteurs();
	void receiveData();
	void sendWebsocket();

private:
	//void problemes();
    Ui::ProjetserreClass ui;
	QTcpSocket* socket;
	QString ip = "192.168.65.8";
	quint16 port = 502;
	QJsonObject donneesJson;
	QWebSocketServer* wSocketServer;
	QWebSocket* wSocket = nullptr;
};
