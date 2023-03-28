#include "Actionneurs.h"

Actionneurs::Actionneurs(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    client = new QModbusTcpClient(this);
    client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
    client->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
	if (!client->connectDevice()) {
		qDebug("connection ratee");
	}
	else {
		qDebug("connection reussie");
	}
	QObject::connect(client, SIGNAL(QModbusDevice::Error errorOccured(err)), this, SLOT(captErreur()));
	QObject::connect(reponse, SIGNAL(QModbusDevice::Error errorOccured(err)), this, SLOT(captErreur()));
}

Actionneurs::~Actionneurs()
{}

void Actionneurs::onConnectButtonClicked()
{
	client->connectDevice();
	if (client->state() == QModbusDevice::ConnectedState) {
		ui.lblConnect->setText("Connecte");
	}
	else if (client->state() == QModbusDevice::ConnectingState) {
		ui.lblConnect->setText("En cours de connection");
	}
	else {
		ui.lblConnect->setText("La carte ne repond pas");
	}
}

void Actionneurs::onDisconnectButtonClicked()
{
	client->disconnectDevice();
	if (client->state() == QModbusDevice::UnconnectedState) {
		ui.lblConnect->setText("Deconnecte");
	}
	else if (client->state() == QModbusDevice::ClosingState) {
		ui.lblConnect->setText("En cours de deconnection");
	}
	else {
		ui.lblConnect->setText("La carte ne repond pas");
	}
}

void Actionneurs::captErreur()
{
	ui.lblError->setText(client->errorString());
	ui.lblError->setText(reponse->errorString());
}

void Actionneurs::actiondel()
{
	reponse->deleteLater();
}

void Actionneurs::Relay1ON()
{
	Relay1.setValue(100, 65280);
	reponse = client->sendWriteRequest(Relay1, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus1->setText("Status : ON");
}

void Actionneurs::Relay2ON()
{
	Relay1.setValue(101, 0xFF00);
	reponse = client->sendWriteRequest(Relay2, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus2->setText("Status : ON");
}

void Actionneurs::Relay3ON()
{
	Relay1.setValue(102, 0xFF00);
	reponse = client->sendWriteRequest(Relay3, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus3->setText("Status : ON");
}

void Actionneurs::Relay4ON()
{
	Relay1.setValue(103, 0xFF00);
	reponse = client->sendWriteRequest(Relay4, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus4->setText("Status : ON");
}

void Actionneurs::Relay1OFF()
{
	Relay1.setValue(100, 0x0000);
	reponse = client->sendWriteRequest(Relay1, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus1->setText("Status : OFF");
}

void Actionneurs::Relay2OFF()
{
	Relay1.setValue(101, 0x0000);
	reponse = client->sendWriteRequest(Relay2, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus2->setText("Status : OFF");
}

void Actionneurs::Relay3OFF()
{
	Relay1.setValue(102, 0x0000);
	reponse = client->sendWriteRequest(Relay3, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus3->setText("Status : OFF");
}

void Actionneurs::Relay4OFF()
{
	Relay1.setValue(103, 0x0000);
	reponse = client->sendWriteRequest(Relay4, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
	ui.lblStatus4->setText("Status : OFF");
}
