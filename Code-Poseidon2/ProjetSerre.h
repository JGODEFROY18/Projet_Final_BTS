#pragma once

#include <QtWidgets/QMainWindow>
#include <QTcpSocket>
#include "ui_ProjetSerre.h"
#include <QtNetwork>
#include <QDataStream>
#include <QLabel>

//#include <wiringPi.h>

class ProjetSerre : public QMainWindow
{
    Q_OBJECT
        QTcpSocket* socket;
public:
    ProjetSerre(QWidget *parent = nullptr);
    ~ProjetSerre();

public slots:
    void onConnectButtonClicked();
    void onSocketConnected();
    void onSocketDisconnected();
    void onDisplayTemperatureClicked();
    void onRelayOnClicked();
    void onRelayOffClicked();
    void activatePompe();
    void EteindrePompe();
    void onSocketReadyRead();
    void onGetLevelClicked();
    void onDisplayDebitClicked();
    void receptionTrame();

private:
    Ui::ProjetSerreClass ui;
    
};
