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
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonDocument doc(donneesJson);
    QByteArray data = doc.toJson();
    QString donnee = data;
    qDebug() << data;
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
    donneesJson.insert("Valeur 1", rand() % 100);
    donneesJson.insert("Valeur 2", rand() % 100);
    donneesJson.insert("Valeur 3", rand() % 100);
    donneesJson.insert("Valeur 4", rand() % 100);
    donneesJson.insert("Valeur 5", rand() % 100);
    donneesJson.insert("Valeur 6", rand() % 100);
    donneesJson.insert("Valeur 7", rand() % 100);
    donneesJson.insert("Valeur 8", rand() % 100);
}
