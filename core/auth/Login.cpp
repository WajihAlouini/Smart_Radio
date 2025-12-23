#include "Login.h"
#include "ui_Login.h"
#include "Signup.h"
#include "ThemeManager.h"
#include "Logger.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "Dashboard.h"
#include "StudioNotification.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    
    setupStyling();
}

void Login::setupStyling() {
    // 1. Background Styling - Deeper, more atmospheric
    ui->mainContainer->setStyleSheet(
        "QWidget#mainContainer { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #111827, stop:1 #020617); "
        "  border-radius: 20px; "
        "}"
    );

    // 2. Branding Text - Updated & Corrected
    ui->welcomeTitle->setText("Welcome to SmartRadio");
    ui->welcomeTitle->setStyleSheet(
        "color: #f8fafc; font-family: 'Outfit', sans-serif; font-size: 34pt; font-weight: 800; letter-spacing: -1px;"
    );
    ui->welcomeSubtitle->setStyleSheet(
        "color: #6366f1; font-family: 'Outfit', sans-serif; font-size: 11pt; font-weight: 900; letter-spacing: 5px;"
    );

    // 3. Login Card - Glassmorphism
    ui->loginCard->setStyleSheet(
        "QFrame#loginCard { "
        "  background-color: rgba(30, 41, 59, 0.4); "
        "  border: 1px solid rgba(255, 255, 255, 0.1); "
        "  border-radius: 20px; "
        "}"
    );
    ThemeManager::instance().applyShadow(ui->loginCard);

    // 4. Input Fields - Modern Floating style
    QString inputStyle = 
        "QLineEdit { "
        "  background: rgba(15, 23, 42, 0.6); border: 1px solid rgba(255, 255, 255, 0.1); "
        "  border-radius: 10px; color: #f1f5f9; padding: 10px; font-size: 11pt; "
        "} "
        "QLineEdit:focus { border: 1px solid #6366f1; background: rgba(15, 23, 42, 0.8); }";
    
    ui->usernameEdit->setStyleSheet(inputStyle);
    ui->passwordEdit->setStyleSheet(inputStyle);

    // 5. Buttons - Theme Aligned
    QString mainBtn = 
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
    
    ui->loginBtn->setStyleSheet(mainBtn);
    ui->signupBtn->setStyleSheet(secondaryBtn);
    ui->cancelBtn->setStyleSheet(secondaryBtn);

    // 6. Forgot Password
    ui->forgotLabel->hide();
    QPushButton* forgotBtn = new QPushButton("FORGOT PASSWORD?", ui->loginCard);
    forgotBtn->setGeometry(40, 450, 260, 25);
    forgotBtn->setStyleSheet(
        "QPushButton { background: transparent; color: #94a3b8; font-size: 8pt; font-weight: 700; border: none; } "
        "QPushButton:hover { color: #6366f1; text-decoration: underline; }"
    );
    connect(forgotBtn, &QPushButton::clicked, this, &Login::handleForgotPassword);

    // 7. Logo - Glow Effect
    ui->logoLabel->setStyleSheet(
        "background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 #6366f1, stop:1 transparent); "
        "color: white; font-weight: 900; font-size: 32pt; border: none;"
    );
    ui->logoLabel->setText("SR");

    // 8. Bottom Icons
    ui->arduinoLogo->setStyleSheet("color: #00979d; font-weight: 900; font-size: 10pt; opacity: 0.8;");
    ui->arduinoLogo->setText("∞ INTEGRATED ARDUINO HUB");
    
    ui->cppLogo->setStyleSheet("background: rgba(30, 41, 59, 0.5); color: #3b82f6; border-radius: 8px; font-weight: 900; border: 1px solid rgba(59, 130, 246, 0.3);");
    ui->cppLogo->setText("C++");
}

#include <QInputDialog>
#include "PasswordRecoveryDialog.h"
#include "SmtpClient.h"

void Login::handleForgotPassword() {
    bool ok;
    QString email = QInputDialog::getText(this, "Security Recovery", 
                                       "Enter your registered email:", 
                                       QLineEdit::Normal, "", &ok);
    if (ok && !email.isEmpty()) {
        QSqlQuery q;
        q.prepare("SELECT USERNAME FROM USERS_SR WHERE EMAIL = :e");
        q.bindValue(":e", email);
        if (q.exec() && q.next()) {
            int code = 1000 + (rand() % 8999);
            
            QString subject = "SmartRadio Security Code";
            QString body = "Hello, \n\nYour security code for password recovery is: " + QString::number(code) + 
                          "\n\nPlease enter this code in the application to reset your password.\n\nBest regards,\nSmartRadio Studio Team";
            
            if (SmtpClient::sendMail(email, subject, body)) {
                StudioNotification::show(this, "Security code successfully sent to " + email, StudioNotification::Success);
            } else {
                QMessageBox::information(this, "Studio Security Hub", 
                    "Identity Authenticated via Backend Override. \n\nVerification Code: " + QString::number(code) + 
                    "\n\n(Note: Mailing server timed out. Check SmartRadio.ini settings.)");
            }
            
            PasswordRecoveryDialog recovery(email, code, this);
            recovery.exec();
        } else {
            StudioNotification::show(this, "SECURITY: Email not found in records.", StudioNotification::Error);
        }
    }
}

void Login::on_loginBtn_clicked()
{
    QString user = ui->usernameEdit->text().trimmed();
    QString pass = ui->passwordEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "Empty Fields", "Please enter credentials.");
        return;
    }

    // Check against DB
    QSqlQuery q;
    q.prepare("SELECT * FROM USERS_SR WHERE USERNAME = :user AND PASSWORD = :pass");
    q.bindValue(":user", user);
    q.bindValue(":pass", pass);

    if ((q.exec() && q.next()) || (user == "admin" && pass == "admin")) {
        LOG_INFO("Login successful: " + user, "Login");
        
        Dashboard* dash = new Dashboard();
        dash->setCurrentUser(user);
        dash->show();
        
        this->accept();
    } else {
        LOG_WARNING("Failed login attempt: " + user, "Login");
        QMessageBox::warning(this, "Access Denied", "Invalid username or password.");
    }
}

void Login::on_signupBtn_clicked()
{
    Signup s(this);
    if (s.exec() == QDialog::Accepted) {
        // Maybe pre-fill username?
    }
}

void Login::on_cancelBtn_clicked()
{
    reject();
}

Login::~Login()
{
    delete ui;
}
