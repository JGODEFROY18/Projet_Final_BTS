#include "Actionneurs.h"

Actionneurs::Actionneurs(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	client = new QModbusTcpClient(this);
	client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "192.168.65.8");
	client->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
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
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0064 FF00"));
	reponse = client->sendRawRequest(request, 100);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus1->setText("Status : ON");
	}
}

void Actionneurs::Relay2ON()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0065 FF00"));
	reponse = client->sendRawRequest(request, 101);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus2->setText("Status : ON");
	}
}

void Actionneurs::Relay3ON()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0066 FF00"));
	reponse = client->sendRawRequest(request, 102);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus3->setText("Status : ON");
	}
}

void Actionneurs::Relay4ON()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0067 FF00"));
	reponse = client->sendRawRequest(request, 103);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus4->setText("Status : ON");
	}
}

void Actionneurs::Relay1OFF()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0064 0000"));
	reponse = client->sendRawRequest(request, 100);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus1->setText("Status : OFF");
	}
}

void Actionneurs::Relay2OFF()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0065 0000"));
	reponse = client->sendRawRequest(request, 101);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus2->setText("Status : OFF");
	}
}

void Actionneurs::Relay3OFF()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0066 0000"));
	reponse = client->sendRawRequest(request, 102);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus3->setText("Status : OFF");
	}
}

void Actionneurs::Relay4OFF()
{
	QModbusRequest request(QModbusRequest::WriteSingleCoil,QByteArray::fromHex("0067 0000"));
	reponse = client->sendRawRequest(request, 103);
	if (!reponse->isFinished()) {
		connect(reponse, SIGNAL(finished()), this, SLOT(actiondel()));
		ui.lblStatus4->setText("Status : OFF");
	}
}
