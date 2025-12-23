#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class ProfileDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProfileDialog(const QString& username, QWidget* parent = nullptr);

protected:
    void showEvent(QShowEvent* event) override;

private slots:
    void on_saveBtn_clicked();

private:
    QString m_username;
    QLineEdit* emailEdit;
    QLineEdit* userEdit; // Pre-filled username
    QLineEdit* currentPassEdit;
    QLineEdit* newPassEdit;
    QLineEdit* confirmPassEdit;
    
    void setupUi();
    void loadUserData();
};

#endif // PROFILEDIALOG_H
