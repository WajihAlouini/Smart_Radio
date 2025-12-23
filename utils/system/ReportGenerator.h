#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QString>
#include <QAbstractItemModel>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QWidget>

class ReportGenerator {
public:
    static bool exportToCsv(QAbstractItemModel* model, QWidget* parent = nullptr) {
        if (!model) return false;

        QString fileName = QFileDialog::getSaveFileName(parent, 
            "Export to CSV", "", "CSV Files (*.csv);;All Files (*)");

        if (fileName.isEmpty()) return false;

        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(parent, "Error", "Could not open file for writing.");
            return false;
        }

        QTextStream out(&file);
        int rows = model->rowCount();
        int cols = model->columnCount();

        // Headers
        for (int i = 0; i < cols; ++i) {
            out << model->headerData(i, Qt::Horizontal).toString();
            if (i < cols - 1) out << ",";
        }
        out << "\n";

        // Data
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                QString data = model->data(model->index(i, j)).toString();
                // Escape commas
                if (data.contains(",")) {
                    data = "\"" + data + "\"";
                }
                out << data;
                if (j < cols - 1) out << ",";
            }
            out << "\n";
        }

        file.close();
        QMessageBox::information(parent, "Success", "Data exported successfully to " + fileName);
        return true;
    }
};

#endif // REPORTGENERATOR_H
