#include "ConfigManager.h"
#include <QApplication>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager()
{
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
    
    configPath = configDir + "/SmartRadio.ini";
    settings = new QSettings(configPath, QSettings::IniFormat);
    
    // Load defaults if first run
    if (!settings->contains("database/driver")) {
        loadDefaults();
    }
}

QString ConfigManager::getDbDriver() const
{
    return settings->value("database/driver", "Oracle in instantclient_23_0").toString();
}

QString ConfigManager::getDbHost() const
{
    return settings->value("database/host", "localhost").toString();
}

QString ConfigManager::getDbPort() const
{
    return settings->value("database/port", "1521").toString();
}

QString ConfigManager::getDbName() const
{
    return settings->value("database/name", "XEPDB1").toString();
}

QString ConfigManager::getDbUser() const
{
    return settings->value("database/user", "SYSTEM").toString();
}

QString ConfigManager::getDbPassword() const
{
    return settings->value("database/password", "").toString();
}

void ConfigManager::setDbDriver(const QString& driver)
{
    settings->setValue("database/driver", driver);
}

void ConfigManager::setDbHost(const QString& host)
{
    settings->setValue("database/host", host);
}

void ConfigManager::setDbPort(const QString& port)
{
    settings->setValue("database/port", port);
}

void ConfigManager::setDbName(const QString& name)
{
    settings->setValue("database/name", name);
}

void ConfigManager::setDbUser(const QString& user)
{
    settings->setValue("database/user", user);
}

void ConfigManager::setDbPassword(const QString& password)
{
    settings->setValue("database/password", password);
}

QString ConfigManager::getSmtpHost() const { return settings->value("smtp/host", "smtp.gmail.com").toString(); }
QString ConfigManager::getSmtpPort() const { return settings->value("smtp/port", "465").toString(); }
QString ConfigManager::getSmtpUser() const { return settings->value("smtp/user", "smartradio.verify@gmail.com").toString(); }
QString ConfigManager::getSmtpPassword() const { return settings->value("smtp/password", "uonj iufh jfzv fypz").toString(); } // Simulation App Password

void ConfigManager::setSmtpHost(const QString& host) { settings->setValue("smtp/host", host); }
void ConfigManager::setSmtpPort(const QString& port) { settings->setValue("smtp/port", port); }
void ConfigManager::setSmtpUser(const QString& user) { settings->setValue("smtp/user", user); }
void ConfigManager::setSmtpPassword(const QString& password) { settings->setValue("smtp/password", password); }

bool ConfigManager::getAutoRefresh() const
{
    return settings->value("app/autoRefresh", false).toBool();
}

void ConfigManager::setAutoRefresh(bool enabled)
{
    settings->setValue("app/autoRefresh", enabled);
}

int ConfigManager::getRefreshInterval() const
{
    return settings->value("app/refreshInterval", 30).toInt();
}

void ConfigManager::setRefreshInterval(int seconds)
{
    settings->setValue("app/refreshInterval", seconds);
}

bool ConfigManager::getNotificationsEnabled() const
{
    return settings->value("app/notificationsEnabled", true).toBool();
}

void ConfigManager::setNotificationsEnabled(bool enabled)
{
    settings->setValue("app/notificationsEnabled", enabled);
}

void ConfigManager::loadDefaults()
{
    // Database defaults (using original credentials as fallback)
    settings->setValue("database/driver", "Oracle in instantclient_23_0");
    settings->setValue("database/host", "localhost");
    settings->setValue("database/port", "1521");
    settings->setValue("database/name", "XEPDB1");
    settings->setValue("database/user", "SYSTEM");
    settings->setValue("database/password", "A123456a"); // Default password for initial setup
    
    // Application defaults
    settings->setValue("app/autoRefresh", false);
    settings->setValue("app/refreshInterval", 30);
    settings->setValue("app/notificationsEnabled", true);
    
    // SMTP defaults
    settings->setValue("smtp/host", "smtp.gmail.com");
    settings->setValue("smtp/port", "465");
    settings->setValue("smtp/user", "smartradio.verify@gmail.com");
    settings->setValue("smtp/password", "uonj iufh jfzv fypz"); // Placeholder for simulation
    
    settings->sync();
}

