#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>
#include <cstdlib>
#include <ctime>

class donnees
{
public:
	donnees();
	void envoi();
	void createJson();

private:
	QJsonObject donneesJson;
	QJsonValue valeur;
};

