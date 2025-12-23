#ifndef GUESTMODULE_H
#define GUESTMODULE_H

#include <QMainWindow>
#include <QtCharts>
#include "Guest.h"
#include "notifications.h"

QT_BEGIN_NAMESPACE
namespace Ui { class GuestModule; }
QT_END_NAMESPACE

class GuestModule : public QMainWindow
{
    Q_OBJECT

public:
    explicit GuestModule(QWidget *parent = nullptr);
    ~GuestModule();

private slots:
    void on_invite_ajout_button_clicked();
    void on_invite_supprime_button_clicked();
    void on_pb_invite_triASC_clicked();
    void on_pb_invite_triDSC_clicked();
    void on_invite_pdf_button_clicked();
    void on_invite_stat_clicked();
    void onSearchTextChanged(const QString &text);
    void handleCsvExport();
    void handleTableSelection(const QModelIndex &index);
    void on_invite_rechercher_button_clicked();
    void on_invite_modifier_button_clicked();
    void on_invite_jeu_reponse_2_clicked();
    void on_invite_jeu_generate_clicked();
    // Mic Control slots
    void on_mic_volume_slider_valueChanged(int value);
    void on_mic_mute_button_clicked();
    void on_mic_test_button_clicked();
    void on_mic_refresh_button_clicked();

private:
    Ui::GuestModule *ui;
    Guest guestService;
    notification notifier;
    int currentQuestionId;
    bool micMuted;
    QTimer *signalTimer;
    void setupMicControl();
    void refreshAudioDevices();
    void updateLiveSignal();
};

#endif // GUESTMODULE_H
