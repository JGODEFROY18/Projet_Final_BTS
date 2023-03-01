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

		QObject::connect(wSocketServer, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
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

void Projetserre::onNewConnection()
{
	wSocket = this->wSocketServer->nextPendingConnection();
	QObject::connect(wSocket, SIGNAL(textMessageReceived(const QString&)), this, SLOT(processTextMessage(const QString&)));
	QObject::connect(wSocket, SIGNAL(disconnected()), this, SLOT(wSocketDisconnected()));

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

void Projetserre::wSocketDisconnected()
{
	qDebug()<< "WebSocket Deconnecte";
}

void Projetserre::AffichageDonnees()
{
	if (socket->state() == QAbstractSocket::ConnectedState) {
		char trame[] = { 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x11, 0x03, 0x4D, 0x58, 0x00, 0x04 };
		QByteArray data(trame, 12);
		socket->write(data);
		socket->flush();
	}
}

void Projetserre::receiveData()
{
	QByteArray data = socket->readAll();
	data = data.right(8);//prend les 8 derniers caracteres
	QByteArray temp = data.left(4);//prend les 4 premiers caracteres
	QByteArray humid = data.right(4);
	donneesJson.insert("Humidité", calc.valeurJson(temp,'H'));
	donneesJson.insert("TempInt", calc.valeurJson(humid,'T'));
	sendWebsocket();
}

void Projetserre::sendWebsocket()
{
	QJsonDocument doc(donneesJson);
	QByteArray docsend = doc.toJson();
	QString data(docsend);
	qDebug() << data;
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
