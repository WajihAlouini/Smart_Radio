#include "StaffModule.h"
#include "ui_StaffModule.h"
#include "Staff.h"
#include "InputValidator.h"
#include "Logger.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QPdfWriter>
#include <QTextDocument>
#include <QPrintDialog>
#include <QSqlQuery>
#include "StudioNotification.h"

StaffModule::StaffModule(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StaffModule)
{
    ui->setupUi(this);
    this->setWindowTitle("SR Studio - Staff Management System");
    this->resize(1200, 850);

    // --- Field Styling ---
    ui->id->setReadOnly(true);
    ui->id->setPlaceholderText("System Assigned");
    ui->label_12->setText("ID (AUTO-GENERATED)");

    // --- Tab Configuration ---
    // (Indices: 0: Hub, 1: Insights, 2: Notes)
    ui->tabWidget->setTabIcon(0, QIcon(":/icons/users.png"));

    // --- Styling & UX Improvements ---
    setupStyling();
    ui->pushButton_enroll->setProperty("accent", true);
    
    // Dropdown Management
    ui->combobox2->clear();
    ui->combobox2->addItems({"General Staff", "Sound Engineer", "Technician", "Radio Host", "Producer", "Radio Host"});

    ui->id->setValidator(new QIntValidator(100, 99999999, this));
    ui->tableView->setModel(staffService.display());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
 
    // Manually connect position selector
    connect(ui->combobox2, QOverload<int>::of(&QComboBox::activated), this, &StaffModule::handlePositionSelected);

     // --- Event Connections ---
    connect(ui->linerech, &QLineEdit::textChanged, this, &StaffModule::onSearchTextChanged);
    
    // Link table selection to the form fields
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, [this]() {
        QModelIndexList selected = ui->tableView->selectionModel()->selectedRows();
        if (!selected.isEmpty()) {
            int row = selected.at(0).row();
            ui->id->setText(ui->tableView->model()->index(row, 0).data().toString());
            ui->nom->setText(ui->tableView->model()->index(row, 1).data().toString());
            ui->prenom->setText(ui->tableView->model()->index(row, 2).data().toString());
            ui->combobox2->setCurrentText(ui->tableView->model()->index(row, 3).data().toString());
            ui->salaire->setText(ui->tableView->model()->index(row, 4).data().toString());
            modifiedPosition = ui->combobox2->currentText();
        }
    });

    // Handle Clear Button
    connect(ui->pushButton_clear, &QPushButton::clicked, [this]() {
        ui->id->clear();
        ui->nom->clear();
        ui->prenom->clear();
        ui->salaire->clear();
        ui->tableView->clearSelection();
    });

    // Aligned CSV Button (Inside Hub List)
    QPushButton* csvBtn = new QPushButton("📊 CSV", ui->groupBox_list);
    csvBtn->setGeometry(540, 40, 100, 40); 
    connect(csvBtn, &QPushButton::clicked, this, &StaffModule::handleCsvExport);

    // Auto-refresh stats when switching to Insights tab
    connect(ui->tabWidget, &QTabWidget::currentChanged, [this](int index) {
        if (index == 1) refreshStats(); // Index 1 is Insights
    });
    
    // Initial Load
    refreshStats();
}

StaffModule::~StaffModule()
{
    delete ui;
}

void StaffModule::on_pushButton_enroll_clicked()
{
    // Input validation (ID skipped as it is auto-incremented)
    if (!InputValidator::validateName(ui->nom->text()) || !InputValidator::validateName(ui->prenom->text())) {
        QMessageBox::warning(this, "Validation Error", "Please provide a valid first and last name.");
        return;
    }
    
    QString firstName = ui->nom->text().trimmed();
    QString lastName = ui->prenom->text().trimmed();
    QString pos = ui->combobox2->currentText();
    int salary = ui->salaire->text().toInt();

    Staff newMember(firstName, lastName, pos, salary);
    if(newMember.add()) {
        LOG_INFO("Staff registered: " + firstName, "StaffModule");
        StudioNotification::show(this, "Staff Registered Successfully", StudioNotification::Success);
        ui->tableView->setModel(staffService.display());
        
        // Clear form after success
        ui->id->clear();
        ui->nom->clear();
        ui->prenom->clear();
        ui->salaire->clear();
    } else {
        QMessageBox::critical(this, "Error", "Registration failed. Please check your connection.");
    }
}

void StaffModule::on_pushButton_supprimer_clicked()
{
    int id = ui->id->text().toInt();
    if(staffService.remove(id)) {
        StudioNotification::show(this, "Staff Record Removed", StudioNotification::Warning);
        ui->tableView->setModel(staffService.display());
    } else {
        StudioNotification::show(this, "Removal Failed", StudioNotification::Error);
    }
}

void StaffModule::on_pushButton_modifier_clicked()
{
    QString id = ui->id->text();
    QString fn = ui->nom->text();
    QString ln = ui->prenom->text();
    QString pos = ui->combobox2->currentText();
    QString sl = ui->salaire->text();

    if(staffService.update(id, fn, ln, pos, sl)) {
        StudioNotification::show(this, "Staff Record Updated", StudioNotification::Success);
        ui->tableView->setModel(staffService.display());
    } else {
        StudioNotification::show(this, "Update Failed", StudioNotification::Error);
    }
}

void StaffModule::on_pushButton_tri_clicked()
{
    ui->tableView->setModel(staffService.sort());
}

// setupStyling is handled by ThemeManager globally, but can be used for local tweaks if needed
void StaffModule::setupStyling() {
    // Local styling tweaks for the hub if needed
}

void StaffModule::on_pushButton_pdf_clicked()
{
    QString strStream;
    QTextStream out(&strStream);
    const int rowCount = ui->tableView->model()->rowCount();
    const int columnCount = ui->tableView->model()->columnCount();

    out << "<html><body bgcolor=#ffffff>"
        << "<h3 style=\"text-align: center;\">Staff Directory Export</h3>"
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
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFileName("Staff_Directory.pdf");
    document->print(&printer);
    delete document;
    QMessageBox::information(this, "Export", "PDF generated as Staff_Directory.pdf");
}

void StaffModule::refreshStats()
{
    QPieSeries *series = new QPieSeries();
    QSqlQuery q("SELECT FONCTION, COUNT(*) FROM PERSONNEL GROUP BY FONCTION");
    
    int total = 0;
    while(q.next()) {
        QString label = q.value(0).toString();
        int count = q.value(1).toInt();
        total += count;
        QPieSlice *slice = series->append(label, count);
        slice->setLabelVisible(true);
        slice->setLabel(QString("%1 (%2)").arg(label).arg(count));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("STUDIO WORKFORCE DISTRIBUTION");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeDark);
    chart->setBackgroundBrush(QBrush(QColor(15, 23, 42))); // Match Dashboard Dark Blue
    
    // Header Font
    QFont titleFont("Outfit", 14, QFont::Bold);
    chart->setTitleFont(titleFont);
    chart->setTitleBrush(QBrush(QColor(255, 255, 255)));

    // Legend Styling
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeCircle);
    chart->legend()->setLabelBrush(QBrush(QColor(226, 232, 240)));

    QChartView *chartview = new QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);
    
    // High-end slice colors (Coral to Deep Blue Palette)
    QList<QColor> colors = {
        QColor(255, 126, 103), // Coral
        QColor(147, 197, 253), // Sky
        QColor(167, 139, 250), // Purple
        QColor(52, 211, 153),  // Emerald
        QColor(251, 191, 36)   // Amber
    };

    int i = 0;
    for(QPieSlice *slice : series->slices()) {
        slice->setBrush(colors[i % colors.size()]);
        slice->setBorderWidth(2);
        slice->setBorderColor(QColor(15, 23, 42));
        i++;
    }

    if(ui->statistique->layout()) {
        QLayoutItem *child;
        while ((child = ui->statistique->layout()->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
    } else {
        ui->statistique->setLayout(new QGridLayout());
    }
    ui->statistique->layout()->setContentsMargins(0, 0, 0, 0);
    ui->statistique->layout()->addWidget(chartview);
}

void StaffModule::on_notepadebt_clicked()
{
    notesWindow = new StudioNotes(this);
    notesWindow->show();
}

// Removed redundant comboBox handler

void StaffModule::handlePositionSelected(int index)
{
    modifiedPosition = ui->combobox2->itemText(index);
}

void StaffModule::onSearchTextChanged(const QString &text)
{
    staffService.search(text, ui->tableView);
}

#include "../../utils/ReportGenerator.h"
void StaffModule::handleCsvExport()
{
    ReportGenerator::exportToCsv(ui->tableView->model(), this);
}
