#include "StudioNotes.h"
#include "ui_StudioNotes.h"
#include <QFileDialog>
#include <QTextStream>

StudioNotes::StudioNotes(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StudioNotes)
{
    ui->setupUi(this);
    this->setWindowTitle("SmartRadio Studio Notepad");
    this->setCentralWidget(ui->textEdit);
    this->resize(800, 600);
}

StudioNotes::~StudioNotes() { delete ui; }

void StudioNotes::on_actionNEW_triggered()
{
    currentFile = "";
    ui->textEdit->setPlainText("");
}

void StudioNotes::on_actionSAVE_triggered()
{
    if (currentFile.isEmpty()) {
        on_actionSAVE_AS_triggered();
        return;
    }
    QFile file(currentFile);
    if(file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << ui->textEdit->toPlainText();
        file.close();
    }
}

void StudioNotes::on_actionSAVE_AS_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Save As");
    if(!file.isEmpty()) {
        currentFile = file;
        on_actionSAVE_triggered();
    }
}

void StudioNotes::on_actionOPEN_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Note");
    if(!file.isEmpty()) {
        QFile sFile(file);
        if(sFile.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&sFile);
            ui->textEdit->setPlainText(in.readAll());
            sFile.close();
            currentFile = file;
        }
    }
}

