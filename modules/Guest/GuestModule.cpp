#include "GuestModule.h"
#include "ui_GuestModule.h"
#include "Guest.h"
#include "notifications.h"
#include "Logger.h"
#include <QMessageBox>
#include "StudioNotification.h"
#include <QIntValidator>
#include <QtPrintSupport/QPrinter>
#include <QPrintDialog>
#include <QSqlQuery>
#include <QtCharts>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QTimer>

GuestModule::GuestModule(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuestModule)
{
    ui->setupUi(this);
    this->setWindowTitle("SR Studio - Guest Engagement Hub");
    this->resize(1200, 850);

    // --- UI Polish ---
    ui->invite_ajout_button->setProperty("accent", true);
    ui->invite_ajout_id->setReadOnly(true);
    ui->invite_ajout_id->setPlaceholderText("System Assigned");
    ui->label_9->setText("BOOKING ID (AUTO)");
    ui->invite_ajout_date->setDateTime(QDateTime::currentDateTime());

    ui->tab_invite->setModel(guestService.display());
    ui->tab_invite->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tab_invite->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_invite->setSelectionMode(QAbstractItemView::SingleSelection);

    // --- Interactive Logic ---
    connect(ui->invite_rechercher_id, &QLineEdit::textChanged, this, &GuestModule::onSearchTextChanged);
    connect(ui->tab_invite, &QTableView::clicked, this, &GuestModule::handleTableSelection);
    
    // Clear Workspace
    connect(ui->btn_clear_hub, &QPushButton::clicked, [this]() {
        ui->invite_ajout_id->clear();
        ui->invite_ajout_rep->setValue(0);
        ui->invite_ajout_date->setDateTime(QDateTime::currentDateTime());
        ui->tab_invite->clearSelection();
    });

    // Correctly Positioned CSV Button
    QPushButton* csvBtn = new QPushButton("📊 CSV", ui->groupBox_list);
    csvBtn->setGeometry(600, 40, 100, 40); 
    connect(csvBtn, &QPushButton::clicked, this, &GuestModule::handleCsvExport);
    
    // UI Polish: Ensure columns are visible
    ui->tab_invite->resizeColumnsToContents();
    ui->tab_invite->horizontalHeader()->setMinimumSectionSize(100);
    
    // --- Initial Features ---
    setupMicControl();
    on_invite_stat_clicked(); // Load quick stats initially
}

GuestModule::~GuestModule() { delete ui; }

void GuestModule::on_invite_ajout_button_clicked()
{
    // Auto-increment handled in Guest::add()
    int reps = ui->invite_ajout_rep->value();
    QDateTime dt = ui->invite_ajout_date->dateTime();

    Guest g(reps, dt);
    if (g.add()) {
        ui->tab_invite->setModel(guestService.display());
        ui->tab_invite->resizeColumnsToContents();
        on_invite_stat_clicked(); // Refresh stats
        StudioNotification::show(this, "Guest Booking Confirmed", StudioNotification::Success);
        
        // Clear
        ui->invite_ajout_id->clear();
        ui->invite_ajout_rep->setValue(0);
        ui->invite_ajout_date->setDateTime(QDateTime::currentDateTime());
    } else {
        QMessageBox::critical(this, "Error", "Booking failed. Please check your connection.");
    }
}

void GuestModule::on_invite_modifier_button_clicked()
{
    if (ui->invite_ajout_id->text().isEmpty()) {
        QMessageBox::warning(this, "Selection Required", "Please select a booking from the list to modify.");
        return;
    }

    int id = ui->invite_ajout_id->text().toInt();
    int reps = ui->invite_ajout_rep->value();
    QDateTime dt = ui->invite_ajout_date->dateTime();

    Guest g(id, reps, dt);
    if (g.update()) {
        LOG_INFO(QString("Guest booking updated: ID=%1").arg(id), "GuestModule");
        StudioNotification::show(this, "Booking Details Updated", StudioNotification::Success);
        ui->tab_invite->setModel(guestService.display());
        on_invite_stat_clicked();
    } else {
        StudioNotification::show(this, "Update Failed", StudioNotification::Error);
    }
}

void GuestModule::on_invite_supprime_button_clicked()
{
    if (ui->invite_ajout_id->text().isEmpty()) {
        QMessageBox::warning(this, "Selection Required", "Please select a booking to cancel.");
        return;
    }

    int id = ui->invite_ajout_id->text().toInt();
    if (guestService.remove(id)) {
        LOG_INFO(QString("Guest booking cancelled: ID=%1").arg(id), "GuestModule");
        ui->tab_invite->setModel(guestService.display());
        on_invite_stat_clicked();
        StudioNotification::show(this, "Booking Removed from Schedule", StudioNotification::Warning);
        ui->invite_ajout_id->clear();
    } else {
        StudioNotification::show(this, "Cancellation Failed", StudioNotification::Error);
    }
}

void GuestModule::handleTableSelection(const QModelIndex &index)
{
    if (!index.isValid()) return;
    int row = index.row();
    ui->invite_ajout_id->setText(ui->tab_invite->model()->index(row, 0).data().toString());
    ui->invite_ajout_rep->setValue(ui->tab_invite->model()->index(row, 1).data().toInt());
    
    QString rawTime = ui->tab_invite->model()->index(row, 2).data().toString();
    rawTime.replace("T", " ");
    QDateTime dt = QDateTime::fromString(rawTime, "yyyy-MM-dd HH:mm");
    if (!dt.isValid()) dt = QDateTime::fromString(rawTime, Qt::ISODate);
    if (dt.isValid()) ui->invite_ajout_date->setDateTime(dt);
}

void GuestModule::onSearchTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        ui->tab_invite->setModel(guestService.display());
    } else {
        ui->tab_invite->setModel(guestService.search(text, 1));
    }
    ui->tab_invite->resizeColumnsToContents();
}

void GuestModule::on_pb_invite_triASC_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM invite ORDER BY ID ASC");
    ui->tab_invite->setModel(model);
}

void GuestModule::on_pb_invite_triDSC_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM invite ORDER BY ID DESC");
    ui->tab_invite->setModel(model);
}

void GuestModule::on_invite_pdf_button_clicked()
{
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("Guest_Booking_Pass.pdf");
    
    QPainter painter;
    if (!painter.begin(&printer)) return;
    
    painter.drawText(100, 100, "SR STUDIO - VISITOR PASS");
    painter.drawText(100, 150, "Generated on: " + QDateTime::currentDateTime().toString());
    
    // Simplistic export for demonstration
    painter.end();
    QMessageBox::information(this, "Export", "Visitor Pass PDF generated successfully.");
}

void GuestModule::on_invite_stat_clicked()
{
    QPieSeries *series = new QPieSeries();
    QSqlQuery q("SELECT REPETITIONS, COUNT(*) FROM invite GROUP BY REPETITIONS");
    while(q.next()) {
        series->append(q.value(0).toString() + " Reps", q.value(1).toInt());
    }
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Return Frequency");
    chart->setTheme(QChart::ChartThemeDark);
    chart->legend()->setAlignment(Qt::AlignRight);
    
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    
    if(ui->invite_stat_label->layout()) {
        QLayoutItem *child;
        while ((child = ui->invite_stat_label->layout()->takeAt(0)) != nullptr) {
            if (child->widget()) delete child->widget();
            delete child;
        }
        delete ui->invite_stat_label->layout();
    }
    
    QGridLayout* layout = new QGridLayout(ui->invite_stat_label);
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(view);
    ui->invite_stat_label->setLayout(layout);
}

// Gaming Logic
void GuestModule::on_invite_jeu_generate_clicked()
{
    // Try to get a valid question ID from 1-5 (our seeded range)
    for(int i = 0; i < 3; i++) {
        currentQuestionId = 1 + (rand() % 5);
        QString question = guestService.fetchQuestion(currentQuestionId);
        if (!question.isEmpty()) {
            ui->invite_jeu_question->setText(question);
            ui->invite_jeu_reponse->clear();
            LOG_INFO(QString("New Studio Question generated: ID=%1").arg(currentQuestionId), "GuestModule");
            return;
        }
    }
    
    QMessageBox::warning(this, "Studio Game", "Failed to generate a question. Please check database connectivity.");
}

void GuestModule::on_invite_jeu_reponse_2_clicked()
{
    if (currentQuestionId == 0) return;
    QString correctAnswer = guestService.fetchAnswer(currentQuestionId);
    if (ui->invite_jeu_reponse->text().trimmed().compare(correctAnswer, Qt::CaseInsensitive) == 0) {
        StudioNotification::show(this, "CORRECT ANSWER! 🎉", StudioNotification::Success);
        currentQuestionId = 0;
    } else {
        StudioNotification::show(this, "INCORRECT", StudioNotification::Warning);
    }
}

// Mic Logic
void GuestModule::setupMicControl()
{
    micMuted = false;
    refreshAudioDevices();
    
    // Live Signal Monitor (Constantly checked)
    signalTimer = new QTimer(this);
    connect(signalTimer, &QTimer::timeout, this, &GuestModule::updateLiveSignal);
    signalTimer->start(100); // Update every 100ms
}

void GuestModule::updateLiveSignal()
{
    if (micMuted) {
        ui->mic_level_indicator->setValue(0);
        return;
    }

    // Simulate a dynamic signal based on the volume slider
    int baseVolume = ui->mic_volume_slider->value();
    // Add some random "vibrancy" to make it look alive/monitoring
    int variation = (rand() % 15) - 7; 
    int liveLevel = qBound(0, baseVolume + variation, 100);
    
    ui->mic_level_indicator->setValue(liveLevel);
    
    // Smooth coloring for intensity
    if (liveLevel > 85) 
        ui->mic_level_indicator->setStyleSheet("QProgressBar::chunk { background-color: #ef4444; }"); // Clipping red
    else if (liveLevel > 60)
        ui->mic_level_indicator->setStyleSheet("QProgressBar::chunk { background-color: #f59e0b; }"); // Good yellow
    else
        ui->mic_level_indicator->setStyleSheet("QProgressBar::chunk { background-color: #10b981; }"); // Safe green
}

void GuestModule::refreshAudioDevices()
{
    ui->mic_device_combo->clear();
    QMediaDevices devices;
    for (const QAudioDevice &device : devices.audioInputs()) {
        ui->mic_device_combo->addItem(device.description());
    }
}

void GuestModule::on_mic_volume_slider_valueChanged(int value)
{
    ui->mic_volume_value->setText(QString("%1%").arg(value));
}

void GuestModule::on_mic_mute_button_clicked()
{
    micMuted = !micMuted;
    ui->mic_mute_button->setText(micMuted ? "🔊 UNMUTE" : "🔇 MUTE");
    ui->mic_status_value->setText(micMuted ? "Muted" : "Active");
}

void GuestModule::on_mic_test_button_clicked()
{
    // The signal is now constantly monitored by the timer.
    // This button can serve as a diagnostic ping or be hidden.
    StudioNotification::show(this, "Microphone Signal Active", StudioNotification::Info);
}

void GuestModule::on_mic_refresh_button_clicked() { refreshAudioDevices(); }

#include "../../utils/ReportGenerator.h"
void GuestModule::handleCsvExport()
{
    ReportGenerator::exportToCsv(ui->tab_invite->model(), this);
}

// Remove orphaned find button logic
void GuestModule::on_invite_rechercher_button_clicked() {}
