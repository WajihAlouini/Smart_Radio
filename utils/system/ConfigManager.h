#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QString>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>

class ConfigManager
{
public:
    static ConfigManager& instance();
    
    // Database configuration
    QString getDbDriver() const;
    QString getDbHost() const;
    QString getDbPort() const;
    QString getDbName() const;
    QString getDbUser() const;
    QString getDbPassword() const;
    
    void setDbDriver(const QString& driver);
    void setDbHost(const QString& host);
    void setDbPort(const QString& port);
    void setDbName(const QString& name);
    void setDbUser(const QString& user);
    void setDbPassword(const QString& password);
    
    // SMTP configuration
    QString getSmtpHost() const;
    QString getSmtpPort() const;
    QString getSmtpUser() const;
    QString getSmtpPassword() const;
    
    void setSmtpHost(const QString& host);
    void setSmtpPort(const QString& port);
    void setSmtpUser(const QString& user);
    void setSmtpPassword(const QString& password);
    
    // Application settings
    bool getAutoRefresh() const;
    void setAutoRefresh(bool enabled);
    
    int getRefreshInterval() const;
    void setRefreshInterval(int seconds);
    
    bool getNotificationsEnabled() const;
    void setNotificationsEnabled(bool enabled);
    
    // Load default configuration if not exists
    void loadDefaults();
    
private:
    ConfigManager();
    ~ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    QSettings* settings;
    QString configPath;
};

#endif // CONFIGMANAGER_H

