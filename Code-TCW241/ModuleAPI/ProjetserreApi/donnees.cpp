#include "donnees.h"

donnees::donnees()
{
    srand(time(NULL));
    createJson();
    envoi();
}
void donnees::envoi()
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QJsonDocument doc(donneesJson);
    QByteArray data = doc.toJson();
    QString donnee = data;
    QUrlQuery requete;
    requete.addQueryItem("donnees", donnee);
    QNetworkReply* reply = mgr->post(request, requete.query().toUtf8());

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
    donneesJson.insert("ConsoElec", rand() % 100);
    donneesJson.insert("ConsoEau", rand() % 100);
    donneesJson.insert("HumidInt", rand() % 100);
    donneesJson.insert("TempInt", rand() % 100);
    donneesJson.insert("TempExt", rand() % 100);
    donneesJson.insert("HumidBac1", rand() % 100);
    donneesJson.insert("HumidBac2", rand() % 100);
    donneesJson.insert("HumidBac3", rand() % 100);
}
