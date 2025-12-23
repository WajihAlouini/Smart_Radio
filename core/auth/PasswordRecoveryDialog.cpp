#include "PasswordRecoveryDialog.h"
#include "ThemeManager.h"
#include "StudioNotification.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QPropertyAnimation>

PasswordRecoveryDialog::PasswordRecoveryDialog(const QString& email, int expectedCode, QWidget* parent)
    : QDialog(parent), m_email(email), m_expectedCode(expectedCode)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(450, 400);
    
    setupUi();
}

void PasswordRecoveryDialog::setupUi() {
    auto mainLayout = new QVBoxLayout(this);
    auto container = new QWidget(this);
    container->setObjectName("mainFrame");
    container->setStyleSheet(
        "QWidget#mainFrame { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0f172a, stop:1 #020617); "
        "  border: 1px solid rgba(99, 102, 241, 0.3); border-radius: 20px; "
        "}"
    );
    mainLayout->addWidget(container);

    auto layout = new QVBoxLayout(container);
    layout->setContentsMargins(30, 30, 30, 30);

    stack = new QStackedWidget(container);
    
    // --- Page 1: Verify Code ---
    auto page1 = new QWidget();
    auto p1Layout = new QVBoxLayout(page1);
    
    auto t1 = new QLabel("VERIFY IDENTITY", page1);
    t1->setStyleSheet("color: white; font-size: 18pt; font-weight: 800;");
    auto s1 = new QLabel("Enter the 4-digit security code sent to your email.", page1);
    s1->setStyleSheet("color: #94a3b8; font-size: 10pt;");
    s1->setWordWrap(true);

    codeEdit = new QLineEdit(page1);
    codeEdit->setPlaceholderText("XXXX");
    codeEdit->setMaxLength(4);
    codeEdit->setAlignment(Qt::AlignCenter);
    codeEdit->setStyleSheet("QLineEdit { background: rgba(30, 41, 59, 0.5); border: 1px solid #6366f1; border-radius: 12px; padding: 15px; color: white; font-size: 20pt; font-weight: 900; letter-spacing: 12px; }");

    auto verifyBtn = new QPushButton("VERIFY CODE", page1);
    verifyBtn->setStyleSheet("QPushButton { background: #6366f1; color: white; border-radius: 10px; padding: 15px; font-weight: 800; } QPushButton:hover { background: #818cf8; }");
    connect(verifyBtn, &QPushButton::clicked, this, &PasswordRecoveryDialog::on_verifyBtn_clicked);

    p1Layout->addWidget(t1);
    p1Layout->addWidget(s1);
    p1Layout->addSpacing(20);
    p1Layout->addWidget(codeEdit);
    p1Layout->addSpacing(20);
    p1Layout->addWidget(verifyBtn);
    p1Layout->addStretch();
    stack->addWidget(page1);

    // --- Page 2: Reset Password ---
    auto page2 = new QWidget();
    auto p2Layout = new QVBoxLayout(page2);
    
    auto t2 = new QLabel("SECURE RESET", page2);
    t2->setStyleSheet("color: white; font-size: 18pt; font-weight: 800;");
    
    QString inputStyle = "QLineEdit { background: rgba(30, 41, 59, 0.5); border: 1px solid rgba(255,255,255,0.1); border-radius: 10px; padding: 12px; color: white; }";
    
    newPassEdit = new QLineEdit(page2); newPassEdit->setEchoMode(QLineEdit::Password); newPassEdit->setPlaceholderText("New Password"); newPassEdit->setStyleSheet(inputStyle);
    confirmPassEdit = new QLineEdit(page2); confirmPassEdit->setEchoMode(QLineEdit::Password); confirmPassEdit->setPlaceholderText("Confirm Password"); confirmPassEdit->setStyleSheet(inputStyle);

    auto resetBtn = new QPushButton("UPDATE PASSWORD", page2);
    resetBtn->setStyleSheet("QPushButton { background: #10b981; color: white; border-radius: 10px; padding: 15px; font-weight: 800; } QPushButton:hover { background: #34d399; }");
    connect(resetBtn, &QPushButton::clicked, this, &PasswordRecoveryDialog::on_resetBtn_clicked);

    p2Layout->addWidget(t2);
    p2Layout->addSpacing(20);
    p2Layout->addWidget(newPassEdit);
    p2Layout->addWidget(confirmPassEdit);
    p2Layout->addSpacing(20);
    p2Layout->addWidget(resetBtn);
    p2Layout->addStretch();
    stack->addWidget(page2);

    layout->addWidget(stack);
    
    auto closeBtn = new QPushButton("CANCEL", container);
    closeBtn->setStyleSheet("QPushButton { background: transparent; color: #64748b; font-size: 8pt; border: none; } QPushButton:hover { color: white; }");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(closeBtn, 0, Qt::AlignCenter);
}

void PasswordRecoveryDialog::on_verifyBtn_clicked() {
    if (codeEdit->text().toInt() == m_expectedCode) {
        stack->setCurrentIndex(1);
        StudioNotification::show(this, "Identity Verified. Please set a new password.", StudioNotification::Success);
    } else {
        StudioNotification::show(this, "Invalid security code.", StudioNotification::Error);
    }
}

void PasswordRecoveryDialog::on_resetBtn_clicked() {
    QString p1 = newPassEdit->text();
    QString p2 = confirmPassEdit->text();

    if (p1.isEmpty() || p1 != p2) {
        StudioNotification::show(this, "Passwords do not match.", StudioNotification::Warning);
        return;
    }

    QSqlQuery up;
    up.prepare("UPDATE USERS_SR SET PASSWORD = :p WHERE EMAIL = :e");
    up.bindValue(":p", p1);
    up.bindValue(":e", m_email);

    if (up.exec()) {
        StudioNotification::show(nullptr, "Password Reset Successfully", StudioNotification::Success);
        accept();
    } else {
        StudioNotification::show(this, "System Error: Failed to reset.", StudioNotification::Error);
    }
}
