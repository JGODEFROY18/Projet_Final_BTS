#pragma once

#include <QtWidgets/QMainWindow>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include "CalculJson.h"
#include "ui_Capteurs.h"

class Capteurs : public QMainWindow
{
    Q_OBJECT

public:
    Capteurs(QWidget *parent = nullptr);
    ~Capteurs();

public slots:
	void onConnectButtonClicked();
	void onDisconnectButtonClicked();
	void captErreur();
	void wSocketConnected();
	void DonneesSensor();
	void DataSensor();
	void DonneesCapteurs();
	void DataCapteurs();
	void sendWebsocket();

private:
    Ui::CapteursClass ui;
    QModbusTcpClient* client;
	QJsonObject donneesJson;
	QWebSocket* wSocket = nullptr;
	QWebSocketServer* wSocketServer;
	CalculJson calc;
	QModbusDataUnit donnees;
	QModbusReply* reponse = nullptr;
	QModbusDevice::Error err = QModbusDevice::NoError;
	
	//création d'une variable contenant le type de registre, son adresse modbus et la taille /2 des données
	QModbusDataUnit donneesBacs = QModbusDataUnit(QModbusDataUnit::HoldingRegisters,17500,6);
	QModbusDataUnit donneesSensor = QModbusDataUnit(QModbusDataUnit::HoldingRegisters,19800,4);
};
