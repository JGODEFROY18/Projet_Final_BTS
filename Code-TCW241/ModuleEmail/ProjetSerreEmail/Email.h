#pragma once
#include <QDateTime>
#include <QTextCodec>
#include <QTcpSocket>

/*
  USAGE!
  Smtp *newMail  = new Smtp("remoteserver","username","pass");
  newMail->Send("from@address.com","to@address.com"," Your Subject","My body text");
  class emit ErrorCloseAll() / SuccessQuit()   or get QStringList ErrorMSG; to read error and data....

*/

class Email : public QObject
{
    Q_OBJECT

public:
    Email(QString smtphost, QString smtpusername, QString smtppass);
    ~Email();
    void setConfig(QString smtphost, QString smtpusername, QString smtppass);
    void send(const QString& from, const QString& to, const QString& subject, const QString& body);
    int linesend;
    QStringList errorMSG;
    QStringList getErrorlist();
    
signals:
    void status(const QString&);
    void connectorSuccess();
    void sendLine();
    void errorCloseAll();
    void successQuit();

private slots:
    void disconnected();
    void connected();
    void readLiner();
    void putSendLine();
    
private:
    bool isconnect;
    QTcpSocket* smtpsocket;
    QTextStream* t;
    QString message, output, RemoteServerName;
    QString smtphost, smtpusername, smtppass;
    QString from, rcpt, response;
    QString mailstatus;
    QString sendLineAndGrab(QString senddata);
    int Timeout;
    QString encodeBase64(QString xml);
    QString decodeBase64(QString xml);
    int dateswap(QString form, uint unixtime);
    QString TimeStampMail();
};
