#include "SponsorshipModule.h"
#include "ui_SponsorshipModule.h"
#include "Sponsor.h"
#include "Logger.h"
#include "InputValidator.h"
#include <QMessageBox>
#include "StudioNotification.h"
#include <QTextStream>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QtCharts>
#include <QInputDialog>
#include <QTime>
#include <QGridLayout>
#include <QComboBox>

SponsorshipModule::SponsorshipModule(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SponsorshipModule)
{
    ui->setupUi(this);
    this->setWindowTitle("SR Studio - Partner & Sponsorship Network");
    this->resize(1200, 850);

    // --- UI Polish ---
    ui->bp_ajouter->setProperty("accent", true);
    ui->le_id->setReadOnly(true);
    ui->le_id->setPlaceholderText("System Assigned");
    
    ui->tabspons->setModel(sponsorService.display());
    ui->tabspons->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tabspons->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabspons->setSelectionMode(QAbstractItemView::SingleSelection);

    // Ensure DB Schema is ready (fix for missing IDs)
    QSqlQuery schemaUpdate;
    schemaUpdate.exec("ALTER TABLE sponsor ADD id NUMBER");
    schemaUpdate.exec("UPDATE sponsor SET id = ROWNUM WHERE id IS NULL");

    // --- Interactive Logic ---
    connect(ui->le_rech, &QLineEdit::textChanged, this, &SponsorshipModule::onSearchTextChanged);
    connect(ui->tabspons, &QTableView::clicked, this, &SponsorshipModule::handleTableSelection);
    
    // Aligned CSV Button (Inside Hub)
    QPushButton* csvBtn = new QPushButton("📊 CSV", ui->groupBox_list);
    csvBtn->setGeometry(490, 40, 100, 40); 
    connect(csvBtn, &QPushButton::clicked, this, &SponsorshipModule::handleCsvExport);

    // Hub Tools
    connect(ui->bp_clear, &QPushButton::clicked, [this]() {
        ui->le_id->clear();
        ui->le_nom->clear();
        ui->le_duree->clear();
        ui->le_heure->clear();
        ui->le_repet_spin->setValue(0);
        ui->tabspons->clearSelection();
    });

    // --- Timer Setup ---
    countdownTimer = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &SponsorshipModule::updateCountdownDisplay);
    minutes = 0; seconds = 0; isPaused = false;
    
    // Style LCDs
    ui->mint->setStyleSheet("background-color: #0f172a; color: #6366f1; border-radius: 8px;");
    ui->secc->setStyleSheet("background-color: #0f172a; color: #6366f1; border-radius: 8px;");

    // Load initial stats
    generateRevenueStats();
}

SponsorshipModule::~SponsorshipModule() { delete ui; }

void SponsorshipModule::on_bp_ajouter_clicked()
{
    if (ui->le_nom->text().isEmpty()) {
        QMessageBox::warning(this, "Validation", "Please provide a sponsor name.");
        return;
    }

    QString name = ui->le_nom->text();
    QString dur = ui->le_duree->text();
    QString air = ui->le_heure->text();
    int rep = ui->le_repet_spin->value();

    Sponsor s(name, dur, air, rep);
    if (s.add()) {
        ui->tabspons->setModel(sponsorService.display());
        ui->tabspons->resizeColumnsToContents();
        generateRevenueStats();
        StudioNotification::show(this, "Partner Registered Successfully", StudioNotification::Success);
        
        // Clear
        ui->le_id->clear();
        ui->le_nom->clear();
        ui->le_duree->clear();
        ui->le_heure->clear();
        ui->le_repet_spin->setValue(0);
    } else {
        QMessageBox::critical(this, "Error", "Failed to register partner.");
    }
}

void SponsorshipModule::on_bp_modifier_clicked()
{
    if (ui->le_id->text().isEmpty()) {
        QMessageBox::warning(this, "Selection Required", "Please select a partner from the log to update.");
        return;
    }

    int id = ui->le_id->text().toInt();
    QString name = ui->le_nom->text();
    QString dur = ui->le_duree->text();
    QString air = ui->le_heure->text();
    int rep = ui->le_repet_spin->value();

    Sponsor s(id, name, dur, air, rep);
    if (s.update(id)) {
        LOG_INFO(QString("Contract updated: ID=%1").arg(id), "SponsorshipModule");
        ui->tabspons->setModel(sponsorService.display());
        generateRevenueStats();
        StudioNotification::show(this, "Contract Updated", StudioNotification::Success);
    } else {
        StudioNotification::show(this, "Update Failed", StudioNotification::Error);
    }
}

void SponsorshipModule::on_bp_supprime_clicked()
{
    if (ui->le_id->text().isEmpty()) {
        QMessageBox::warning(this, "Selection Required", "Please select a partner to discontinue.");
        return;
    }

    int id = ui->le_id->text().toInt();
    if (sponsorService.remove(id)) {
        LOG_INFO(QString("Contract terminated: ID=%1").arg(id), "SponsorshipModule");
        ui->tabspons->setModel(sponsorService.display());
        generateRevenueStats();
        StudioNotification::show(this, "Partnership Terminated", StudioNotification::Warning);
        ui->le_id->clear();
    } else {
        StudioNotification::show(this, "Termination Failed", StudioNotification::Error);
    }
}

void SponsorshipModule::handleTableSelection(const QModelIndex &index)
{
    if (!index.isValid()) return;
    int row = index.row();
    ui->le_id->setText(ui->tabspons->model()->index(row, 0).data().toString());
    ui->le_nom->setText(ui->tabspons->model()->index(row, 1).data().toString());
    ui->le_duree->setText(ui->tabspons->model()->index(row, 2).data().toString());
    ui->le_heure->setText(ui->tabspons->model()->index(row, 3).data().toString());
    ui->le_repet_spin->setValue(ui->tabspons->model()->index(row, 4).data().toInt());
}

void SponsorshipModule::onSearchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        ui->tabspons->setModel(sponsorService.display());
    } else {
        ui->tabspons->setModel(sponsorService.search(text));
    }
    ui->tabspons->resizeColumnsToContents();
}

void SponsorshipModule::on_bp_tri_clicked()
{
    ui->tabspons->setModel(sponsorService.sort());
}

void SponsorshipModule::on_bp_pdf_clicked()
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("Sponsorship_Contracts.pdf");
    QPainter painter;
    if (painter.begin(&printer)) {
        painter.drawText(100, 100, "SR STUDIO - SPONSORSHIP REPORT");
        painter.end();
        QMessageBox::information(this, "PDF Export", "Report generated successfully.");
    }
}

void SponsorshipModule::on_tabWidget_currentChanged(int index)
{
    if (index == 1) generateRevenueStats();
}

void SponsorshipModule::generateRevenueStats()
{
    QPieSeries *series = new QPieSeries();
    QSqlQuery q("SELECT nom, repetition FROM sponsor");
    while(q.next()) {
        series->append(q.value(0).toString(), q.value(1).toInt());
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Campaign Distribution");
    chart->setTheme(QChart::ChartThemeDark);
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);

    if(ui->label_stat->layout()) {
       QLayoutItem *item;
       while((item = ui->label_stat->layout()->takeAt(0)) != nullptr) {
           delete item->widget();
           delete item;
       }
       delete ui->label_stat->layout();
    }
    ui->label_stat->setLayout(new QGridLayout());
    ui->label_stat->layout()->addWidget(view);
}

// Timer logic
void SponsorshipModule::on_bp_play_clicked()
{
    bool ok;
    int input = QInputDialog::getInt(this, "Set Timer", "Minutes:", 1, 1, 60, 1, &ok);
    if(ok) {
        minutes = input;
        seconds = 0;
        ui->mint->display(minutes);
        ui->secc->display(seconds);
        countdownTimer->start(1000);
    }
}

void SponsorshipModule::on_bp_pause_clicked()
{
    if(isPaused) {
        countdownTimer->start(1000);
        ui->bp_pause->setText("PAUSE");
    } else {
        countdownTimer->stop();
        ui->bp_pause->setText("RESUME");
    }
    isPaused = !isPaused;
}

void SponsorshipModule::on_bp_stop_clicked()
{
    countdownTimer->stop();
    minutes = 0; seconds = 0;
    ui->mint->display(0);
    ui->secc->display(0);
}

void SponsorshipModule::updateCountdownDisplay()
{
    if (seconds == 0) {
        if (minutes == 0) {
            countdownTimer->stop();
            StudioNotification::show(this, "AIRTIME ENDED: Sponsorship Slot Complete", StudioNotification::Info);
            return;
        }
        minutes--;
        seconds = 59;
    } else {
        seconds--;
    }
    ui->mint->display(minutes);
    ui->secc->display(seconds);
}

#include "../../utils/ReportGenerator.h"
void SponsorshipModule::handleCsvExport()
{
    ReportGenerator::exportToCsv(ui->tabspons->model(), this);
}

void SponsorshipModule::on_bp_rech_clicked() {} // Redundant
void SponsorshipModule::on_bp_aller_clicked() { generateRevenueStats(); }
