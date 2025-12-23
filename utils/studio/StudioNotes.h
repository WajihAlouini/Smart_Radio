#ifndef STUDIONOTES_H
#define STUDIONOTES_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>

namespace Ui {
class StudioNotes;
}

class StudioNotes : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudioNotes(QWidget *parent = nullptr);
    ~StudioNotes();

private slots:
    void on_actionSAVE_triggered();
    void on_actionOPEN_triggered();
    void on_actionSAVE_AS_triggered();
    void on_actionNEW_triggered();

private:
    Ui::StudioNotes *ui;
    QString currentFile;
};

#endif // STUDIONOTES_H
