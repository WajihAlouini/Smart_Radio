#ifndef BROADCASTSTATS_H
#define BROADCASTSTATS_H

#include <QDialog>
#include "qcustomplot.h"

namespace Ui {
class BroadcastStats;
}

class BroadcastStats : public QDialog
{
    Q_OBJECT

public:
    explicit BroadcastStats(QWidget *parent = nullptr);
    ~BroadcastStats();
    void createChart();

private:
    Ui::BroadcastStats *ui;
};

#endif // BROADCASTSTATS_H
