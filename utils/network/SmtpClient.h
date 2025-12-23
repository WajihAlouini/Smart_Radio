#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QEventLoop>

class SmtpClient : public QObject
{
    Q_OBJECT
public:
    static bool sendMail(const QString &to, const QString &subject, const QString &body) {
        SmtpClient client;
        return client.execute(to, subject, body);
    }

private:
    bool execute(const QString &to, const QString &subject, const QString &body);
};

#endif // SMTPCLIENT_H
