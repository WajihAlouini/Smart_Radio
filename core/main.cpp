#include "StaffModule.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QString>
#include <QDebug>
#include <QIcon>
#include "Dashboard.h"
#include "Login.h"
#include "Logger.h"
#include "ConfigManager.h"
#include "ThemeManager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icons/AppIcon.png"));
    
    // Initialize logging system
    Logger::instance().info("SmartRadio application starting", "Main");
    
    // Initialize configuration (loads defaults if first run)
    ConfigManager::instance();
    LOG_INFO("Configuration system initialized", "Main");

    // --- Premium Studio Interface ---
    a.setStyleSheet(ThemeManager::instance().getGlobalStyleSheet());

    Connection c;
    if(c.createconnect())
    {
        // Show Login Interface First
        Login login;
        if (login.exec() == QDialog::Accepted) {
            Dashboard *dash = new Dashboard();
            dash->show();
            LOG_INFO("User authenticated, Dashboard displayed", "Main");
        } else {
            LOG_INFO("Login cancelled or failed, exiting", "Main");
            return 0;
        }
    }
    else 
    {
        LOG_CRITICAL(QString("Database connection failed: %1").arg(c.getLastErrorText()), "Main");
        QMessageBox::critical(nullptr, QObject::tr("Engine Error"),
                    "Database synchronization failed:\n" + c.getLastErrorText() + 
                    "\n\nPlease check your database configuration in the settings file.", 
                    QMessageBox::Cancel);
        return -1;
    }

    LOG_INFO("Application entering event loop", "Main");
    return a.exec();
}
