#include "Capteurs.h"

Capteurs::Capteurs(QWidget* parent)
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
	wSocketServer = new QWebSocketServer(QStringLiteral("Server WebSocket"), QWebSocketServer::NonSecureMode);
	QObject::connect(client, SIGNAL(QModbusDevice::Error errorOccured(err)), this, SLOT(captErreur()));
	QObject::connect(reponse, SIGNAL(QModbusDevice::Error errorOccured(err)), this, SLOT(captErreur()));

	if (this->wSocketServer->listen(QHostAddress::AnyIPv4, 12345))
	{
		QObject::connect(wSocketServer, SIGNAL(newConnection()), this, SLOT(wSocketConnected()));
	}
	else
	{
		qDebug() << "Server WebSocket: Erreur d'ecoute sur le port 12345" << "\n";
	}
}

Capteurs::~Capteurs()
{}

void Capteurs::onConnectButtonClicked()
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

void Capteurs::onDisconnectButtonClicked()
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

void Capteurs::captErreur()
{
	ui.lblError->setText(client->errorString());
	ui.lblError->setText(reponse->errorString());
}

void Capteurs::wSocketConnected()
{
	wSocket = this->wSocketServer->nextPendingConnection();
	qDebug() << "Nouvelle connection";
}

void Capteurs::DonneesSensor()
{
	reponse = client->sendReadRequest(donneesSensor, 1);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(DataSensor()));
}

void Capteurs::DataSensor()
{
	donnees = reponse->result();
	quint16 temp[2] = { donnees.value(0),donnees.value(1) };
	quint16 humid[2] = { donnees.value(2),donnees.value(3) };
	donneesJson.insert("TempInt", calc.valeurJson(temp, 'T', false));
	donneesJson.insert("HumidInt", calc.valeurJson(humid, 'H', false));
	ui.lblHumid->setText(donneesJson.value("HumidInt").toString());
	ui.lblTemp->setText(donneesJson.value("TempInt").toString());
	sendWebsocket();
	reponse->deleteLater();
}

void Capteurs::DonneesCapteurs()
{
	reponse = client->sendReadRequest(donneesBacs, 2);
	if (!reponse->isFinished())
		connect(reponse, SIGNAL(finished()), this, SLOT(DataCapteurs()));
}

void Capteurs::DataCapteurs()
{
	donnees = reponse->result();
	quint16 humids1[2] = { donnees.value(0),donnees.value(1) };
	quint16 humids2[2] = { donnees.value(2),donnees.value(3) };
	quint16 humids3[2] = { donnees.value(4),donnees.value(5) };
	donneesJson.insert("HumidSol1", calc.valeurJson(humids1, 'H', true));
	donneesJson.insert("HumidSol2", calc.valeurJson(humids2, 'H', true));
	donneesJson.insert("HumidSol3", calc.valeurJson(humids3, 'H', true));
	ui.lblHumid_2->setText(donneesJson.value("HumidSol1").toString());
	ui.lblHumid_3->setText(donneesJson.value("HumidSol2").toString());
	ui.lblHumid_4->setText(donneesJson.value("HumidSol3").toString());
	sendWebsocket();
	reponse->deleteLater();
}

void Capteurs::sendWebsocket()
{
	QJsonDocument doc(donneesJson);
	QByteArray docsend = doc.toJson();
	QString data(docsend);
	if (wSocket != nullptr) wSocket->sendTextMessage(data);
}
