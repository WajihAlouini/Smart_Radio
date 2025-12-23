#ifndef SPONSORSHIPMODULE_H
#define SPONSORSHIPMODULE_H

#include <QMainWindow>
#include <QTimer>
#include "Sponsor.h"

namespace Ui {
class SponsorshipModule;
}

class SponsorshipModule : public QMainWindow
{
    Q_OBJECT

public:
    explicit SponsorshipModule(QWidget *parent = nullptr);
    ~SponsorshipModule();

private slots:
    void on_bp_ajouter_clicked();
    void on_bp_modifier_clicked();
    void on_bp_supprime_clicked();
    void on_bp_tri_clicked();
    void on_bp_rech_clicked();
    void on_bp_pdf_clicked();
    void onSearchTextChanged(const QString &text);
    void handleCsvExport();
    void handleTableSelection(const QModelIndex &index);
    void on_bp_aller_clicked();
    void on_bp_play_clicked();
    void on_bp_pause_clicked();
    void on_bp_stop_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::SponsorshipModule *ui;
    Sponsor sponsorService;
    int minutes, seconds;
    bool isPaused;
    QTimer *countdownTimer;  // Airtime countdown timer
    void updateCountdownDisplay();
    void generateRevenueStats();
};

#endif // SPONSORSHIPMODULE_H
