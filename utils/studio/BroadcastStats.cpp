#include "BroadcastStats.h"
#include "ui_BroadcastStats.h"
#include <QSqlQuery>

BroadcastStats::BroadcastStats(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BroadcastStats)
{
    ui->setupUi(this);
    this->setWindowTitle("Broadcast Performance Analytics");
    createChart();
}

BroadcastStats::~BroadcastStats() { delete ui; }

void BroadcastStats::createChart()
{
    ui->customPlot->setBackground(QBrush(QColor(15, 23, 42)));

    QCPBars *bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    bars->setAntialiased(false);
    bars->setName("Audience Views by Show Category");
    bars->setPen(QPen(QColor(99, 102, 241).lighter(150)));
    bars->setBrush(QColor(99, 102, 241));

    QVector<double> ticks;
    QVector<QString> labels;
    QSqlQuery q("SELECT TYPE, SUM(NB_VIEW) FROM EMISSION GROUP BY TYPE");
    int i = 0;
    QVector<double> data;
    double maxValue = 0;
    
    while(q.next()) {
        i++;
        ticks << i;
        labels << q.value(0).toString();
        double value = q.value(1).toDouble();
        data << value;
        if (value > maxValue) {
            maxValue = value;
        }
    }

    if (i == 0) {
        // No data available
        ui->customPlot->xAxis->setRange(0, 1);
        ui->customPlot->yAxis->setRange(0, 100);
    } else {
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(ticks, labels);
        ui->customPlot->xAxis->setTicker(textTicker);
        ui->customPlot->xAxis->setTickLabelRotation(45);
        ui->customPlot->xAxis->setRange(0, i + 1);
        
        // Dynamic Y-axis range with padding
        double yMax = maxValue * 1.15; // 15% padding
        if (yMax < 10) yMax = 10; // Minimum range
        ui->customPlot->yAxis->setRange(0, yMax);
    }
    
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setLabel("Total Views");
    ui->customPlot->yAxis->setLabelColor(Qt::white);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);

    bars->setData(ticks, data);
    ui->customPlot->replot();
}
