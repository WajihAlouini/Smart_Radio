#include "Signup.h"
#include "ui_Signup.h"
#include "ThemeManager.h"
#include "Logger.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "StudioNotification.h"

Signup::Signup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupStyling();
}

void Signup::setupStyling() {
    ui->mainContainer->setStyleSheet(
        "QWidget#mainContainer { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #111827, stop:1 #020617); "
        "  border-radius: 20px; "
        "}"
    );

    ui->welcomeTitle->setText("Join SmartRadio");
    ui->welcomeTitle->setStyleSheet(
        "color: #f8fafc; font-family: 'Outfit', sans-serif; font-size: 34pt; font-weight: 800; letter-spacing: -1px;"
    );
    ui->welcomeSubtitle->setStyleSheet(
        "color: #6366f1; font-family: 'Outfit', sans-serif; font-size: 11pt; font-weight: 900; letter-spacing: 5px;"
    );

    ui->signupCard->setStyleSheet(
        "QFrame#signupCard { "
        "  background-color: rgba(30, 41, 59, 0.4); "
        "  border: 1px solid rgba(255, 255, 255, 0.1); "
        "  border-radius: 20px; "
        "}"
    );
    ThemeManager::instance().applyShadow(ui->signupCard);

    QString inputStyle = 
        "QLineEdit { "
        "  background: rgba(15, 23, 42, 0.6); border: 1px solid rgba(255, 255, 255, 0.1); "
        "  border-radius: 10px; color: #f1f5f9; padding: 10px; font-size: 11pt; "
        "} "
        "QLineEdit:focus { border: 1px solid #6366f1; background: rgba(15, 23, 42, 0.8); }";
    
    ui->usernameEdit->setStyleSheet(inputStyle);
    ui->emailEdit->setStyleSheet(inputStyle);
    ui->passwordEdit->setStyleSheet(inputStyle);

    QString btnStyle = 
        "QPushButton { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #6366f1, stop:1 #4f46e5); "
        "  color: white; border-radius: 10px; font-weight: 800; font-size: 11pt; "
        "} "
        "QPushButton:hover { background: #818cf8; }";
    
    QString secondaryBtn = 
        "QPushButton { "
        "  background: rgba(255, 255, 255, 0.05); color: #94a3b8; border-radius: 10px; "
        "  font-weight: 700; border: 1px solid rgba(255, 255, 255, 0.1); "
        "} "
        "QPushButton:hover { background: rgba(255, 255, 255, 0.1); color: white; }";
    
    ui->createBtn->setStyleSheet(btnStyle);
    ui->backBtn->setStyleSheet(secondaryBtn);

    ui->logoLabel->setStyleSheet(
        "background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #6366f1, stop:1 transparent); "
        "color: white; font-weight: 900; font-size: 32pt; border: none;"
    );
    ui->logoLabel->setText("SR");
}

void Signup::on_createBtn_clicked()
{
    QString user = ui->usernameEdit->text().trimmed();
    QString email = ui->emailEdit->text().trimmed();
    QString pass = ui->passwordEdit->text();

    if (user.isEmpty() || email.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Empty Fields", "Please fill in all details.");
        return;
    }

    QSqlQuery q;
    // Attempt to create the table if it doesn't exist (using a simple users table for SmartRadio)
    q.exec("CREATE TABLE USERS_SR (USERNAME VARCHAR2(50) PRIMARY KEY, EMAIL VARCHAR2(100), PASSWORD VARCHAR2(50))");

    q.prepare("INSERT INTO USERS_SR (USERNAME, EMAIL, PASSWORD) VALUES (:user, :email, :pass)");
    q.bindValue(":user", user);
    q.bindValue(":email", email);
    q.bindValue(":pass", pass);

    if (q.exec()) {
        LOG_INFO("New user registered: " + user, "Signup");
        StudioNotification::show(nullptr, "Account Created! You can now log in.", StudioNotification::Success);
        accept();
    } else {
        LOG_ERROR("Registration failed: " + q.lastError().text(), "Signup");
        StudioNotification::show(nullptr, "User ID Taken or DB Error", StudioNotification::Error);
    }
}

void Signup::on_backBtn_clicked()
{
    reject();
}

Signup::~Signup()
{
    delete ui;
}
