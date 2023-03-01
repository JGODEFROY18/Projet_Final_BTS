#pragma once
#include <QtWidgets/QMainWindow>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class CalculJson
{
	float QByteArrayToFloat(QByteArray arr);
public:
	
	QJsonValue valeurJson(QByteArray,char);
	float calculHumidite(int);
};

