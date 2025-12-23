#ifndef PASSWORDRECOVERYDIALOG_H
#define PASSWORDRECOVERYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>

class PasswordRecoveryDialog : public QDialog {
    Q_OBJECT
public:
    explicit PasswordRecoveryDialog(const QString& email, int expectedCode, QWidget* parent = nullptr);

private slots:
    void on_verifyBtn_clicked();
    void on_resetBtn_clicked();

private:
    QString m_email;
    int m_expectedCode;
    
    QStackedWidget* stack;
    
    // Page 1: Code Entry
    QLineEdit* codeEdit;
    
    // Page 2: New Password
    QLineEdit* newPassEdit;
    QLineEdit* confirmPassEdit;
    
    void setupUi();
};

#endif // PASSWORDRECOVERYDIALOG_H
