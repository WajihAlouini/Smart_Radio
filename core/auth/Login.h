#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_loginBtn_clicked();
    void on_signupBtn_clicked();
    void on_cancelBtn_clicked();
    void handleForgotPassword();

private:
    Ui::Login *ui;
    void setupStyling();
};

#endif // LOGIN_H
