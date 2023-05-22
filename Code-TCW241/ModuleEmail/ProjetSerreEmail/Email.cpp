#include "Email.h"

Email::Email(QString smtphost, QString smtpusername, QString smtppass)
{
    qDebug() << "### Class Smtp start";
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass;
}
void Email::setConfig(QString smtphost, QString smtpusername, QString smtppass)
{
    this->smtphost = smtphost;
    this->smtpusername = smtpusername;
    this->smtppass = smtppass;
}

void Email::send(const QString& from, const QString& to, const QString& subject, const QString& body)
{
    int waittime = 5 * 1000;
    this->from = from;
    rcpt = to;
    errorMSG.clear();
    Timeout = waittime;
    linesend = 0;
    isconnect = false;

    message = TimeStampMail() + "\n";
    message.append("User-Agent: Mozilla Thunderbird 1.0.6 (Macintosh/20050716)\n");
    message.append("X-Accept-Language: zh_CN, zh_CN\n");
    message.append("MIME-Version: 1.0\n");
    message.append("To: " + to + "\n");
    message.append("From: " + from + " <" + from + ">\n");
    message.append("Subject: " + subject + "\n");
    message.append("Content-Type: text/html; charset=UTF8;\n");   /* or txt */
    message.append("Content-transfer-encoding: 7BIT\n\n\n\n");
    message.append(body);
    message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    smtpsocket = new QTcpSocket(this);
    connect(this, SIGNAL(connectorSuccess()), this, SLOT(readLiner()));
    connect(this, SIGNAL(sendLine()), this, SLOT(putSendLine()));
    if (smtphost.size() > 0)
    {
        smtpsocket->connectToHost("smtp.gmail.com", 25);
        qDebug() << "meh";
    }
    else
    {
        qDebug() << "mele7";
        smtpsocket->connectToHost("localhost", 25);
    }
    if (smtpsocket->waitForConnected(Timeout))
    {
        if (smtpsocket->waitForReadyRead(Timeout))
        {
            isconnect = true;
            emit connectorSuccess();
        }
    }
    else
    {
        emit errorCloseAll();
        qDebug() << "---close all ---";
    }

}

void Email::readLiner()
{
    if (isconnect)
    {
        QTextCodec* codecx;
        codecx = QTextCodec::codecForMib(106);
        t = new QTextStream(smtpsocket);
        t->setCodec(codecx);

        int loops = 0;
        while (!t->atEnd())
        {
            loops++;
            response = t->readLine();
        }
        if (response.size() > 0)
        {
            RemoteServerName = response;
            mailstatus = response.left(3);
            if (mailstatus == "220")
            {
                response = "";
                linesend = 1;
                emit sendLine();
            }
        }
        else
        {
            emit errorCloseAll();
            qDebug() << "here";
        }
    }
}

Email::~Email()

{
}

/* LINE SENDER  */
void Email::putSendLine()
{
    int current = linesend;
    switch (current)
    {
    case 1:
        response = sendLineAndGrab("ehlo " + smtphost);
        if (response.size() > 0)
        {
            errorMSG.append(response);
            linesend = 2;
            emit sendLine();
        }
        response = "";
        break;
    case 2:
        response = sendLineAndGrab("AUTH LOGIN");
        if (response.size() >= 0)
        {
            errorMSG.append(response);
            linesend = 3;
            emit sendLine();
        }

        response = "";

        break;
    case 3:
        response = sendLineAndGrab(encodeBase64(smtpusername));   /* username send */
        if (response.size() > 0)
        {
            errorMSG.append(response);
            linesend = 4;
            emit sendLine();
        }

        response = "";
        break;
    case 4:
        response = sendLineAndGrab(encodeBase64(smtppass));     /* pass send */
        qDebug() << smtppass;
        qDebug() << "4---- " << response;
        if (response.size() > 0)
        {
            errorMSG.append(response);
            if (response.contains("235", Qt::CaseInsensitive))
            {
                linesend = 5;
                emit sendLine();
            }

        }

        response = "";
        break;
    case 5:
        response = sendLineAndGrab("MAIL FROM:<" + from + ">");
        if (response.size() > 0)
        {
            linesend = 6;
            emit sendLine();
        }

        break;
    case 6:
        response = sendLineAndGrab("RCPT TO: <" + rcpt + ">");
        if (response.size() > 0)
        {
            errorMSG.append(response);
            response = "";
            response = sendLineAndGrab("DATA");
            if (!response.contains("not", Qt::CaseInsensitive))
            {
                errorMSG.append(response);
                response = "";
                linesend = 7;
                emit sendLine();
            }
        }
        response = "";
        break;
    case 7:
        response = sendLineAndGrab(message + "\r\n.");

        if (response.size() && response.contains("ok", Qt::CaseInsensitive))
        {
            errorMSG.append(response);
            linesend = 8;
            emit sendLine();
        }
        response = "";
        break;
    case 8:
        sendLineAndGrab("QUIT");
        break;
    default:
        /*emit ErrorCloseAll();*/
        return;
        break;
    }
}

/* SENDER AND RECIVER  */
QString Email::sendLineAndGrab(QString senddata)
{
    QString incommingData = "";
    if (isconnect)
    {
        /////////QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        int current = linesend;
        int loops = 0;

        *t << senddata << "\r\n";
        t->flush();
        if (senddata != "QUIT") {
            if (smtpsocket->waitForReadyRead(Timeout))
            {
                while (!t->atEnd())
                {
                    loops++;
                    QString opera = t->readLine() + "\n";
                    incommingData = opera + incommingData;
                }
            }
        }
        else
        {
            ////////QApplication::restoreOverrideCursor();
            delete smtpsocket;
            delete t;
            isconnect = false;
            emit successQuit();
            return incommingData;
        }
        ///////QApplication::restoreOverrideCursor();
    }
    else
    {
        emit errorCloseAll();
    }
    return incommingData;
}

void Email::disconnected()
{
    qDebug() << "disconneted";
    qDebug() << "error " << smtpsocket->errorString();
}
void Email::connected()
{
    output.append("connected");
}

QString Email::encodeBase64(QString xml)
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString Email::decodeBase64(QString xml)
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}


QStringList Email::getErrorlist()
{
    return errorMSG;
}

int Email::dateswap(QString form, uint unixtime)
{
    QDateTime fromunix;
    fromunix.setTime_t(unixtime);
    QString numeric = fromunix.toString((const QString)form);
    bool ok;
    return (int)numeric.toFloat(&ok);
}


QString Email::TimeStampMail()
{
    /* mail rtf Date format! http://www.faqs.org/rfcs/rfc788.html */
    QDateTime timer1(QDateTime::currentDateTime());


    uint unixtime = timer1.toTime_t();
    QDateTime fromunix;
    fromunix.setTime_t(unixtime);


    QStringList RTFdays = QStringList() << "giorno_NULL" << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    QStringList RTFmonth = QStringList() << "mese_NULL" << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    QDate timeroad(dateswap("yyyy", unixtime), dateswap("M", unixtime), dateswap("d", unixtime));

    QStringList rtfd_line;
    rtfd_line.clear();
    rtfd_line.append("Date: ");
    rtfd_line.append(RTFdays.at(timeroad.dayOfWeek()));
    rtfd_line.append(", ");
    rtfd_line.append(QString::number(dateswap("d", unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(RTFmonth.at(dateswap("M", unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(QString::number(dateswap("yyyy", unixtime)));
    rtfd_line.append(" ");
    rtfd_line.append(fromunix.toString("hh:mm:ss"));
    rtfd_line.append(" +0100");

    return QString(rtfd_line.join(""));
}