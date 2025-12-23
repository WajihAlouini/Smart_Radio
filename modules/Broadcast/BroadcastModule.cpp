#include "BroadcastModule.h"
#include "ui_BroadcastModule.h"
#include "Broadcast.h"
#include "BroadcastStats.h"
#include "BroadcastHistory.h"
#include "InputValidator.h"
#include "Logger.h"
#include <QMessageBox>
#include <QTextStream>
#include <QTextDocument>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QPageSize>
#include <QPageLayout>
#include <QIntValidator>
#include "StudioNotification.h"

BroadcastModule::BroadcastModule(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BroadcastModule)
{
    ui->setupUi(this);
    this->setWindowTitle("SR Studio - Broadcast Operations Center");
    this->resize(1200, 850); 
    
    // --- UI Polish ---
    ui->pushButton->setProperty("accent", true);
    ui->lineEdit_ID->setReadOnly(true);
    ui->lineEdit_ID->setPlaceholderText("System Assigned");
    ui->label->setText("ID (AUTO-GENERATED)");
    ui->dateTimeEdit_Heure->setDateTime(QDateTime::currentDateTime());
    
    ui->tableView->setModel(broadcastService.display());
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    
    // Header Tuning: Ensure columns like AIR TIME show full content
    QHeaderView *header = ui->tableView->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Interactive);
    header->setSectionResizeMode(1, QHeaderView::Stretch); // Stretch the Name column
    ui->tableView->resizeColumnsToContents();
    header->setMinimumSectionSize(100);

    // --- Interactive Logic ---
    connect(ui->lineEdit_Recherche, &QLineEdit::textChanged, this, &BroadcastModule::onSearchTextChanged);
    connect(ui->tableView, &QTableView::clicked, this, &BroadcastModule::handleTableSelection);
    connect(ui->tableView_Calendar, &QTableView::clicked, this, &BroadcastModule::handleTableSelection);

    // Handle Clear Button
    connect(ui->pushButton_4, &QPushButton::clicked, [this]() {
        ui->lineEdit_ID->clear();
        ui->lineEdit_Nom->clear();
        ui->dateTimeEdit_Heure->setDateTime(QDateTime::currentDateTime());
        ui->lineEdit_Type->clear();
        ui->lineEdit_Duree->clear();
        ui->lineEdit_NBView->clear();
        ui->tableView->clearSelection();
    });

    // Auto-refresh when switching tabs
    connect(ui->tabWidget, &QTabWidget::currentChanged, [this](int index) {
        if (index == 1) { // Calendar tab
            on_calendarWidget_selectionChanged();
        } else if (index == 2) { // History tab
            ui->tableView_hist->setModel(broadcastHistory.display());
            ui->tableView_hist->resizeColumnsToContents();
            ui->tableView_hist->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
    });

    ui->pushButton_6->setText("🔄 REFRESH HISTORY");
    ui->pushButton_6->setProperty("secondary", true);
    ui->tableView_hist->horizontalHeader()->setVisible(true);
    ui->tableView_hist->setAlternatingRowColors(true);

    // Hub CSV Export
    QPushButton* csvBtn = new QPushButton("📊 CSV", ui->groupBox_list);
    csvBtn->setGeometry(490, 40, 100, 40); 
    connect(csvBtn, &QPushButton::clicked, this, &BroadcastModule::handleCsvExport);

    // Initial load for Hub
    // Hub UI Tuning
    ui->tableView->setModel(broadcastService.display());
    ui->tableView->resizeColumnsToContents();
    
    // Calendar UI Tuning
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    ui->calendarWidget->setGridVisible(true);
    ui->tableView_Calendar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_Calendar->setAlternatingRowColors(true);
}

BroadcastModule::~BroadcastModule() { delete ui; }

void BroadcastModule::on_pushButton_clicked()
{
    // Input validation (ID skipped for registration)
    if (!InputValidator::validateName(ui->lineEdit_Nom->text())) {
        QMessageBox::warning(this, "Validation Error", InputValidator::getErrorMessage());
        return;
    }
    
    QString name = ui->lineEdit_Nom->text().trimmed();
    QString time = ui->dateTimeEdit_Heure->dateTime().toString("yyyy-MM-dd HH:mm");
    QString type = ui->lineEdit_Type->text().trimmed();
    int duration = ui->lineEdit_Duree->text().toInt();
    int views = ui->lineEdit_NBView->text().toInt();

    Broadcast newShow(name, time, type, duration, views);
    if (newShow.add()) {
        ui->tableView->setModel(broadcastService.display());
        ui->tableView->resizeColumnsToContents();
        
        // Log to history (fetch the newly created ID)
        QSqlQuery idCheck("SELECT MAX(ID) FROM EMISSION");
        if(idCheck.next()) {
            int newId = idCheck.value(0).toInt();
            BroadcastHistory historyEntry(type, newId, name);
            historyEntry.addEntry();
        }

        LOG_INFO("Broadcast show added: " + name, "BroadcastModule");
        StudioNotification::show(this, "Broadcast Show Scheduled", StudioNotification::Success);
        
        // Clear form
        ui->lineEdit_ID->clear();
        ui->lineEdit_Nom->clear();
        ui->dateTimeEdit_Heure->setDateTime(QDateTime::currentDateTime());
        ui->lineEdit_Type->clear();
        ui->lineEdit_Duree->clear();
        ui->lineEdit_NBView->clear();
    } else {
        QMessageBox::critical(this, "Error", "Scheduling failed. Please check your connection.");
    }
}

void BroadcastModule::on_pushButton_Supprimer_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    if (broadcastService.remove(id)) {
        ui->tableView->setModel(broadcastService.display());
        ui->tableView->resizeColumnsToContents();
        StudioNotification::show(this, "Show Removed from Schedule", StudioNotification::Warning);
    } else {
        StudioNotification::show(this, "Removal Failed", StudioNotification::Error);
    }
}

void BroadcastModule::on_pushButton_2_clicked()
{
    // Input validation
    if (!InputValidator::validateId(ui->lineEdit_ID->text())) {
        QMessageBox::warning(this, "Validation Error", InputValidator::getErrorMessage());
        return;
    }
    
    if (!InputValidator::validateName(ui->lineEdit_Nom->text())) {
        QMessageBox::warning(this, "Validation Error", InputValidator::getErrorMessage());
        return;
    }
    
    int id = ui->lineEdit_ID->text().toInt();
    QString name = ui->lineEdit_Nom->text().trimmed();
    QString time = ui->dateTimeEdit_Heure->dateTime().toString("yyyy-MM-dd HH:mm");
    QString type = ui->lineEdit_Type->text().trimmed();
    int duration = ui->lineEdit_Duree->text().toInt();
    int views = ui->lineEdit_NBView->text().toInt();

    if (broadcastService.update(id, name, time, type, duration, views)) {
        LOG_INFO(QString("Broadcast show updated: ID=%1").arg(id), "BroadcastModule");
        StudioNotification::show(this, "Show Details Updated", StudioNotification::Success);
        ui->tableView->setModel(broadcastService.display());
        ui->tableView->resizeColumnsToContents();
    } else {
        LOG_ERROR(QString("Failed to update broadcast show: ID=%1").arg(id), "BroadcastModule");
        StudioNotification::show(this, "Update Failed", StudioNotification::Error);
    }
}

// Actualiser removed from UI, log is live searched.

void BroadcastModule::on_pushButton_Chercher_clicked()
{
    ui->tableView->setModel(broadcastService.search(ui->lineEdit_Recherche->text()));
    ui->tableView->resizeColumnsToContents();
}

// Sorting simplified to default log view.

void BroadcastModule::on_pushButton_5_clicked()
{
    QString strStream;
    QTextStream out(&strStream);
    const int rowCount = ui->tableView->model()->rowCount();
    const int columnCount = ui->tableView->model()->columnCount();

    out << "<html><body bgcolor=#ffffff>"
        << "<h3 style=\"text-align: center;\">Broadcast Logistics Report</h3>"
        << "<table border=1 cellspacing=0 cellpadding=5 width=\"100%\">";

    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead><tbody>";

    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString();
            out << QString("<td>%1</td>").arg(data);
        }
        out << "</tr>";
    }
    out << "</table></body></html>";

    QTextDocument *document = new QTextDocument();
    document->setHtml(strStream);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("Broadcast_Log.pdf");
    document->print(&printer);
    delete document;
    QMessageBox::information(this, "Export", "PDF Report saved as Broadcast_Log.pdf");
}

void BroadcastModule::on_calendarWidget_selectionChanged()
{
    QDate selectedDate = ui->calendarWidget->selectedDate();
    ui->lineEdit_Calendrier->setText(selectedDate.toString("yyyy-MM-dd"));
    ui->tableView_Calendar->setModel(broadcastService.displayCalendar(selectedDate));
    ui->tableView_Calendar->resizeColumnsToContents();
}

void BroadcastModule::on_pushButton_6_clicked()
{
    ui->tableView_hist->setModel(broadcastHistory.display());
}

void BroadcastModule::onSearchTextChanged(const QString &text)
{
    ui->tableView->setModel(broadcastService.search(text));
    ui->tableView->resizeColumnsToContents();
}

#include "../../utils/ReportGenerator.h"
void BroadcastModule::handleCsvExport()
{
    ReportGenerator::exportToCsv(ui->tableView->model(), this);
}

void BroadcastModule::handleTableSelection(const QModelIndex &index)
{
    if (!index.isValid()) return;
    
    int row = index.row();
    const QAbstractItemModel* model = index.model();
    
    ui->lineEdit_ID->setText(model->index(row, 0).data().toString());
    ui->lineEdit_Nom->setText(model->index(row, 1).data().toString());
    
    // Air Time Handling
    QString rawTime = model->index(row, 2).data().toString();
    rawTime.replace("T", " "); 
    QDateTime dt = QDateTime::fromString(rawTime, "yyyy-MM-dd HH:mm");
    if (!dt.isValid()) dt = QDateTime::fromString(rawTime, Qt::ISODate);
    if (dt.isValid()) ui->dateTimeEdit_Heure->setDateTime(dt);
    
    ui->lineEdit_Type->setText(model->index(row, 3).data().toString());
    ui->lineEdit_Duree->setText(model->index(row, 4).data().toString());
    ui->lineEdit_NBView->setText(model->index(row, 5).data().toString());
}
