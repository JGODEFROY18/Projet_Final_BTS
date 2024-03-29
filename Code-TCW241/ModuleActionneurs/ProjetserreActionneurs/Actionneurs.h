#pragma once

#include <QtWidgets/QMainWindow>
#include <QModbusTcpClient>
#include <QModbusReply>
#include <QModbusRequest>
#include "ui_Actionneurs.h"

class Actionneurs : public QMainWindow
{
    Q_OBJECT

public:
    Actionneurs(QWidget *parent = nullptr);
    ~Actionneurs();

public slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void captErreur();
	void actiondel();
	void Relay1ON();
	void Relay2ON();
	void Relay3ON();
	void Relay4ON();
	void Relay1OFF();
	void Relay2OFF();
	void Relay3OFF();
	void Relay4OFF();

private:
    Ui::ActionneursClass ui;
	QModbusTcpClient* client;
	QModbusReply* reponse = nullptr;
	QModbusDevice::Error err = QModbusDevice::NoError;
	QModbusDataUnit Relay1 = QModbusDataUnit(QModbusDataUnit::Coils, 100, 1);
	QModbusDataUnit Relay2 = QModbusDataUnit(QModbusDataUnit::Coils, 101, 1);
	QModbusDataUnit Relay3 = QModbusDataUnit(QModbusDataUnit::Coils, 102, 1);
	QModbusDataUnit Relay4 = QModbusDataUnit(QModbusDataUnit::Coils, 103, 1);
};
