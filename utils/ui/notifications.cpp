#include "notifications.h"
#include "ConfigManager.h"
#include "Logger.h"
#include <QSystemTrayIcon>
#include <QString>
#include <QApplication>

// Static instance for reuse
static QSystemTrayIcon* notifyIcon = nullptr;

notification::notification()
{
    // Initialize system tray icon once
    if (!notifyIcon && QSystemTrayIcon::isSystemTrayAvailable()) {
        notifyIcon = new QSystemTrayIcon(QApplication::instance());
        notifyIcon->setIcon(QApplication::windowIcon());
        notifyIcon->show();
    }
}

void notification::notification_ajout()
{
    if (!ConfigManager::instance().getNotificationsEnabled()) {
        return;
    }
    
    if (notifyIcon && QSystemTrayIcon::isSystemTrayAvailable()) {
        notifyIcon->showMessage(
            "SmartRadio - Staff Added",
            "Staff member has been successfully registered.",
            QSystemTrayIcon::Information,
            5000
        );
        LOG_INFO("Notification: Staff member added", "Notifications");
    }
}

void notification::notification_supprimer()
{
    if (!ConfigManager::instance().getNotificationsEnabled()) {
        return;
    }
    
    if (notifyIcon && QSystemTrayIcon::isSystemTrayAvailable()) {
        notifyIcon->showMessage(
            "SmartRadio - Staff Removed",
            "Staff member record has been deleted from the system.",
            QSystemTrayIcon::Information,
            5000
        );
        LOG_INFO("Notification: Staff member removed", "Notifications");
    }
}

void notification::notification_modifier()
{
    if (!ConfigManager::instance().getNotificationsEnabled()) {
        return;
    }
    
    if (notifyIcon && QSystemTrayIcon::isSystemTrayAvailable()) {
        notifyIcon->showMessage(
            "SmartRadio - Staff Updated",
            "Staff member information has been successfully updated.",
            QSystemTrayIcon::Information,
            5000
        );
        LOG_INFO("Notification: Staff member updated", "Notifications");
    }
}
