#include "donnees.h"

void donnees::envoi()
{
	QNetworkAccessManager* mgr = new QNetworkAccessManager();
	const QUrl url("http://192.168.64.158/Serre/fonctions/test.php");
	QNetworkRequest request(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QJsonDocument doc(donneesJson);
	QByteArray data = doc.toJson();
	QNetworkReply* reply = mgr->post(request, data);

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
	qDebug() << "sortie de fonction";
}

void donnees::createJson()
{
	donneesJson.insert("Valeur 1", rand() % 100);
	donneesJson.insert("Valeur 2", rand() % 100);
	donneesJson.insert("Valeur 3", rand() % 100);
	donneesJson.insert("Valeur 4", rand() % 100);
	donneesJson.insert("Valeur 5", rand() % 100);
	donneesJson.insert("Valeur 6", rand() % 100);
	donneesJson.insert("Valeur 7", rand() % 100);
	donneesJson.insert("Valeur 8", rand() % 100);
}

donnees::donnees()
{
	srand(time(NULL));
	createJson();
	qDebug()<<donneesJson;
	envoi();
}
