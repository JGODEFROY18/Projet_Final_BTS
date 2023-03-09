#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
#include "ui_ProjetSerre.h"
//#include <wiringPi.h>

class ProjetSerre : public QMainWindow
{
    Q_OBJECT

public:
    ProjetSerre(QWidget *parent = nullptr);
    ~ProjetSerre();

public slots:
    void onConnectButtonClicked();
    void onSocketConnected();
    void onSocketDisconnected();
    void onDisplayTemperatureClicked();
    void onSocketReadyRead();

private:
    Ui::ProjetSerreClass ui;
    QTcpSocket* socket;
};
