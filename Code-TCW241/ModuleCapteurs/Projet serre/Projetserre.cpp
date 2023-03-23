#include "Projetserre.h"

Projetserre::Projetserre(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	socket = new QTcpSocket(this);
	wSocketServer = new QWebSocketServer(QStringLiteral("Server WebSocket"), QWebSocketServer::NonSecureMode);
	QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
	QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));

	if (this->wSocketServer->listen(QHostAddress::AnyIPv4, 12345))
	{
		qDebug() << "Server WebSocket: Nouvelle connexion sur le port 12345" << "\n";

		QObject::connect(wSocketServer, SIGNAL(newConnection()), this, SLOT(wSocketConnected()));
	}
	else
	{
		qDebug() << "Server WebSocket: Erreur d'ecoute sur le port 12345" << "\n";
	}
	//chercher envoi http post url("http:"//server/fichier.php")
}

Projetserre::~Projetserre()
{
}

void Projetserre::onConnectButtonClicked()
{
	socket->connectToHost(ip, port);
	if (!socket->waitForConnected(1000))
		ui.lblConnect->setText("La carte n'est pas allumée");
}

void Projetserre::onDisconnectButtonClicked()
{
	socket->disconnectFromHost();
}

void Projetserre::onSocketConnected()
{
	ui.lblConnect->setText("Connecte");
}

void Projetserre::onSocketDisconnected()
{
	ui.lblConnect->setText("Deconnecte");
}

void Projetserre::wSocketConnected()
{
	wSocket = this->wSocketServer->nextPendingConnection();
	qDebug() << "Nouvelle connection";
}

void Projetserre::wSocketDisconnected()
{
}

void Projetserre::DonneesSensor()
{
	if (socket->state() == QAbstractSocket::ConnectedState) {
		char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x03, 0x4D, 0x58, 0x00, 0x04 };
		QByteArray data(trame, 12);
		socket->write(data);
		socket->flush();
	}
}

void Projetserre::DonneesCapteurs()
{
	if (socket->state() == QAbstractSocket::ConnectedState) {
		//description =  4268 en 64 octet; valeur = 445C
		char trame[] = { 0x00, 0x02, 0x00, 0x00, 0x00, 0x06, 0x11, 0x03, 0x44, 0x5C, 0x00, 0x08 };
		QByteArray data(trame, 12);
		socket->write(data);
		socket->flush();
	}
}

void Projetserre::receiveData()
{
	CalculJson calc;
	QByteArray data = socket->readAll();
	char test = data[1];
	if (test == 1) {
		data = data.right(8);//prend les 8 derniers caracteres
		QByteArray temp = data.left(4);//prend les 4 premiers caracteres
		QByteArray humid = data.right(4);
		donneesJson.insert("HumidInt", calc.valeurJson(humid, 'H'));
		donneesJson.insert("TempInt", calc.valeurJson(temp, 'T'));
		ui.lblHumid->setText(donneesJson.value("HumidInt").toString());
		ui.lblTemp->setText(donneesJson.value("TempInt").toString());
	}
	else if (test == 2) {
		data = data.right(16);//prend les 8 derniers caracteres
		qDebug() << data;
		QByteArray humids1 = data.left(4);//prend les 4 premiers caracteres
		data.remove(0, 4);
		qDebug() << data;
		QByteArray humids2 = data.left(4);
		data.remove(0, 4);
		qDebug() << data;
		QByteArray humids3 = data.left(4);
		QByteArray val4 = data.right(4);
		donneesJson.insert("HumidSol1", calc.valeurJson(humids1, 'H'));
		donneesJson.insert("HumidSol2", calc.valeurJson(humids2, 'H'));
		donneesJson.insert("HumidSol3", calc.valeurJson(humids3, 'H'));
		donneesJson.insert("Valeur4", calc.valeurJson(val4, 'D'));
		ui.lblHumid_2->setText(donneesJson.value("HumidSol1").toString());
		ui.lblHumid_3->setText(donneesJson.value("HumidSol2").toString());
		ui.lblHumid_4->setText(donneesJson.value("HumidSol3").toString());
	}
	if(wSocket != nullptr) sendWebsocket();
}

void Projetserre::sendWebsocket()
{
	QJsonDocument doc(donneesJson);
	qDebug() << doc;
	QByteArray docsend = doc.toJson();
	QString data(docsend);
	wSocket->sendTextMessage(data);
}

/*void Projetserre::problemes()
{
	QNetworkAccessManager* mgr = new QNetworkAccessManager(this);
	const QUrl url(QStringLiteral("http:/192.168.64.158/Serre/fonctions/api.json"));
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonDocument document(donneesJson);
	QByteArray donnees = document.toJson();
	QNetworkReply* reply = mgr->post(request, donnees);

	QObject::connect(reply, &QNetworkReply::finished, [=]() {
		if (reply->error() == QNetworkReply::NoError) {
			QString contents = QString::fromUtf8(reply->readAll());
			qDebug() << contents;
		}
		else {
			QString err = reply->errorString();
			qDebug() << err;
		}
	reply->deleteLater();
		});
}*/
