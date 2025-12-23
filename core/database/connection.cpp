#include "connection.h"
#include "../utils/ConfigManager.h"
#include "../utils/Logger.h"
#include <QSqlError>
#include <QDebug>
#include <QStandardPaths>

Connection::Connection()
{

}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    
    // Load configuration from ConfigManager
    ConfigManager& config = ConfigManager::instance();
    QString driver = config.getDbDriver();
    QString host = config.getDbHost();
    QString port = config.getDbPort();
    QString dbName = config.getDbName();
    QString user = config.getDbUser();
    QString password = config.getDbPassword();
    
    // Check if password is set, use default if empty
    if (password.isEmpty()) {
        // Try default password as fallback
        password = "A123456a";
        LOG_WARNING("Using default password. Please configure your own password in the config file.", "Connection");
    }
    
    // Build connection string
    QString connectionString = QString("Driver={%1};Dbq=%2:%3/%4;Uid=%5;Pwd=%6;")
                               .arg(driver)
                               .arg(host)
                               .arg(port)
                               .arg(dbName)
                               .arg(user)
                               .arg(password);
    
    db.setDatabaseName(connectionString);

    if (db.open()) {
        test = true;
        LOG_INFO("Database connected successfully", "Connection");
        qDebug() << "Database connected successfully using configuration!";
    } else {
        QSqlError error = db.lastError();
        lastError = QString("Database connection failed:\n\n"
                           "Error: %1\n\n"
                           "Please verify:\n"
                           "1. Oracle database is running\n"
                           "2. ODBC driver '%2' is installed\n"
                           "3. Connection details in config file are correct\n"
                           "4. Database is accessible at %3:%4")
                           .arg(error.text())
                           .arg(driver)
                           .arg(host)
                           .arg(port);
        LOG_ERROR(QString("Database connection failed: %1").arg(error.text()), "Connection");
        qDebug() << "Connection Error:" << error.text();
        qDebug() << "Connection String:" << connectionString;
    }

    return test;
}
