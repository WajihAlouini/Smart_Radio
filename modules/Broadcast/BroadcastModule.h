#ifndef BROADCASTMODULE_H
#define BROADCASTMODULE_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include "Broadcast.h"
#include "BroadcastStats.h"
#include "BroadcastHistory.h"
#include "qcustomplot.h"

namespace Ui {
class BroadcastModule;
}

class BroadcastModule : public QMainWindow
{
    Q_OBJECT

public:
    explicit BroadcastModule(QWidget *parent = nullptr);
    ~BroadcastModule();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_Supprimer_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_Chercher_clicked();
    void on_pushButton_5_clicked();
    void on_calendarWidget_selectionChanged();
    void on_pushButton_6_clicked();
    void onSearchTextChanged(const QString &text);
    void handleCsvExport();
    void handleTableSelection(const QModelIndex &index);

private:
    Ui::BroadcastModule *ui;
    Broadcast broadcastService;
    BroadcastStats *statsWindow;
    BroadcastHistory broadcastHistory;
};

#endif // BROADCASTMODULE_H
