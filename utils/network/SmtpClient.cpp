#include "SmtpClient.h"
#include "ConfigManager.h"

bool SmtpClient::execute(const QString &to, const QString &subject, const QString &body)
{
    QString host = ConfigManager::instance().getSmtpHost();
    int port = ConfigManager::instance().getSmtpPort().toInt();
    if (port == 0) port = 465; // Safe default
    QString user = ConfigManager::instance().getSmtpUser();
    QString pass = ConfigManager::instance().getSmtpPassword();

    QSslSocket socket;
    socket.connectToHostEncrypted(host, port);
    if (!socket.waitForEncrypted(10000)) {
        qDebug() << "SMTP Error: SSL Connection timeout or failed." << socket.errorString();
        return false;
    }

    auto getResponse = [&]() {
        if (!socket.waitForReadyRead(10000)) return QString("");
        QString res = QString::fromUtf8(socket.readAll());
        qDebug() << "SMTP Server:" << res;
        return res;
    };

    auto send = [&](const QString &cmd) {
        qDebug() << "SMTP Client:" << (cmd.contains("AUTH") ? "AUTH LOG" : cmd);
        socket.write(cmd.toUtf8() + "\r\n");
        socket.flush();
        return getResponse();
    };

    if (!getResponse().startsWith("220")) return false;
    if (!send("EHLO localhost").contains("250")) return false;
    if (!send("AUTH LOGIN").contains("334")) return false;
    if (!send(user.toUtf8().toBase64()).contains("334")) return false;
    if (!send(pass.toUtf8().toBase64()).contains("235")) {
        qDebug() << "SMTP Error: Authentication failed.";
        return false;
    }

    send("MAIL FROM:<" + user + ">");
    send("RCPT TO:<" + to + ">");
    if (!send("DATA").contains("354")) return false;
    
    QString msg = "To: " + to + "\nFrom: " + user + "\nSubject: " + subject + "\n\n" + body + "\r\n.\r\n";
    if (!send(msg).contains("250")) return false;
    
    send("QUIT");
    socket.close();
    return true;
}
