#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include "StaffModule.h"
#include "BroadcastModule.h"
#include "SponsorshipModule.h"
#include "GuestModule.h"

namespace Ui {
class Dashboard;
}

class Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void updateDateTime();
    void refreshStats();
    void on_logoutBtn_clicked();
    void on_profileBtn_clicked();

public:
    void setCurrentUser(const QString& user);

private:
    Ui::Dashboard *ui;
    QString currentUser;
    QLabel *userLabel;
    QPushButton *logoutBtn;
    StaffModule *staffModule;
    BroadcastModule *broadcastModule;
    SponsorshipModule *sponsorshipModule;
    GuestModule *guestModule;
    
    QTimer *updateTimer;
    QLabel *clockLabel;
    QLabel *statusIndicator;
    
    // Stats labels
    QLabel *staffCountLabel;
    QLabel *broadcastCountLabel;
    QLabel *sponsorCountLabel;
    
    void setupEnhancedUI();
};

#endif // DASHBOARD_H
