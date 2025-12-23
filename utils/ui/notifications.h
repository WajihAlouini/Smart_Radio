#ifndef NOTIFICATIONS_H
#define NOTIFICATIONS_H

#include <QSystemTrayIcon>

class notification
{
public:
    notification();
    void notification_ajout(); // Keeping these for now to minimize changes in other modules until I sweep them
    void notification_supprimer();
    void notification_modifier();
    
    // Better English aliases
    void notifyAdd() { notification_ajout(); }
    void notifyDelete() { notification_supprimer(); }
    void notifyUpdate() { notification_modifier(); }
};

#endif // NOTIFICATIONS_H
