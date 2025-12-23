#ifndef STAFFMODULE_H
#define STAFFMODULE_H

#include <QMainWindow>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QTcpSocket>
#include "Staff.h"
#include "StudioNotes.h"
#include "notifications.h"
#include "Arduino.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StaffModule; }
QT_END_NAMESPACE

class StaffModule : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffModule(QWidget *parent = nullptr);
    ~StaffModule();

private slots:
    void on_pushButton_enroll_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_tri_clicked();
    void on_pushButton_pdf_clicked();
    void refreshStats();
    void on_notepadebt_clicked();
    void handlePositionSelected(int index);
    void onSearchTextChanged(const QString &text);
    void handleCsvExport();

private:
    Ui::StaffModule *ui;
    Staff staffService;
    StudioNotes *notesWindow;
    notification notifier;
    Arduino arduinoController;
    QString selectedPosition, modifiedPosition;
    void setupStyling();
};

#endif // STAFFMODULE_H
