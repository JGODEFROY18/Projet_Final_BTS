#pragma once
#include <QtWidgets/QMainWindow>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class CalculJson
{
	float quintToFloat(quint16*);
	float calculHumidite(float);

public:	
	QJsonValue valeurJson(quint16*,char,bool);
};

