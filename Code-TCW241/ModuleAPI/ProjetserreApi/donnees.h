#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <cstdlib>
#include <ctime>

class donnees : public QObject
{
    Q_OBJECT

public:
    donnees();
    void envoi();
    void createJson();

private:

    QJsonObject donneesJson;
    QJsonValue valeur;
    QUrl url = QUrl("http://192.168.64.158/Serre/fonctions/test.php");
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
};
