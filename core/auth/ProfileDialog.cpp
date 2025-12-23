#include "ProfileDialog.h"
#include "ThemeManager.h"
#include "StudioNotification.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFormLayout>
#include <QGroupBox>

ProfileDialog::ProfileDialog(const QString& username, QWidget* parent) 
    : QDialog(parent), m_username(username) 
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(500, 650);
    
    setupUi();
    loadUserData();
}

void ProfileDialog::setupUi() {
    auto mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    auto container = new QWidget(this);
    container->setObjectName("mainFrame");
    container->setStyleSheet(
        "QWidget#mainFrame { "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0f172a, stop:1 #020617); "
        "  border: 1px solid rgba(99, 102, 241, 0.3); "
        "  border-radius: 24px; "
        "}"
    );
    mainLayout->addWidget(container);

    auto layout = new QVBoxLayout(container);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(25);

    // Header
    auto header = new QVBoxLayout();
    auto title = new QLabel("USER PROFILE", container);
    title->setStyleSheet("color: #f8fafc; font-family: 'Outfit'; font-size: 24pt; font-weight: 800; letter-spacing: -1px;");
    
    auto subtitle = new QLabel("Update your studio credentials and identity", container);
    subtitle->setStyleSheet("color: #6366f1; font-weight: 700; font-size: 9pt; text-transform: uppercase; letter-spacing: 2px;");
    
    header->addWidget(title);
    header->addWidget(subtitle);
    layout->addLayout(header);

    // Form fields
    QString inputStyle = 
        "QLineEdit { "
        "  background: rgba(30, 41, 59, 0.5); border: 1px solid rgba(255, 255, 255, 0.1); "
        "  border-radius: 12px; padding: 12px; color: #f1f5f9; font-size: 11pt; "
        "} "
        "QLineEdit:focus { border: 1px solid #6366f1; background: rgba(30, 41, 59, 0.8); } "
        "QLineEdit:read-only { color: #64748b; background: rgba(15, 23, 42, 0.3); }";

    userEdit = new QLineEdit(container); userEdit->setStyleSheet(inputStyle); userEdit->setReadOnly(true);
    emailEdit = new QLineEdit(container); emailEdit->setStyleSheet(inputStyle);
    currentPassEdit = new QLineEdit(container); currentPassEdit->setStyleSheet(inputStyle); currentPassEdit->setEchoMode(QLineEdit::Password);
    newPassEdit = new QLineEdit(container); newPassEdit->setStyleSheet(inputStyle); newPassEdit->setEchoMode(QLineEdit::Password);
    confirmPassEdit = new QLineEdit(container); confirmPassEdit->setStyleSheet(inputStyle); confirmPassEdit->setEchoMode(QLineEdit::Password);

    auto addField = [&](const QString& label, QLineEdit* edit) {
        auto l = new QLabel(label, container);
        l->setStyleSheet("color: #94a3b8; font-weight: 700; font-size: 9pt; margin-bottom: -15px;");
        layout->addWidget(l);
        layout->addWidget(edit);
    };

    addField("STUDIO USERNAME", userEdit);
    addField("CONTACT EMAIL", emailEdit);
    
    auto line = new QFrame(container);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background: rgba(255,255,255,0.05);");
    layout->addWidget(line);

    addField("CURRENT PASSWORD (REQUIRED)", currentPassEdit);
    addField("NEW PASSWORD", newPassEdit);
    addField("CONFIRM NEW PASSWORD", confirmPassEdit);

    layout->addStretch();

    // Buttons
    auto btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(15);

    auto saveBtn = new QPushButton("SAVE UPDATES", container);
    saveBtn->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #6366f1, stop:1 #4f46e5); color: white; border-radius: 12px; padding: 15px; font-weight: 800; font-size: 11pt; } "
        "QPushButton:hover { background: #818cf8; }"
    );
    connect(saveBtn, &QPushButton::clicked, this, &ProfileDialog::on_saveBtn_clicked);

    auto cancelBtn = new QPushButton("EXIT", container);
    cancelBtn->setStyleSheet(
        "QPushButton { background: rgba(255, 255, 255, 0.05); color: #94a3b8; border-radius: 12px; padding: 15px; font-weight: 700; border: 1px solid rgba(255, 255, 255, 0.1); } "
        "QPushButton:hover { background: rgba(255, 255, 255, 0.1); color: white; }"
    );
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    btnLayout->addWidget(cancelBtn, 1);
    btnLayout->addWidget(saveBtn, 2);
    layout->addLayout(btnLayout);

    ThemeManager::instance().applyShadow(container);
}

void ProfileDialog::loadUserData() {
    userEdit->setText(m_username);
    QSqlQuery q;
    q.prepare("SELECT EMAIL FROM USERS_SR WHERE USERNAME = :u");
    q.bindValue(":u", m_username);
    if(q.exec() && q.next()) {
        emailEdit->setText(q.value(0).toString());
    }
}

void ProfileDialog::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);
    setWindowOpacity(0.0);
    QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(400);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void ProfileDialog::on_saveBtn_clicked() {
    QString curPass = currentPassEdit->text();
    QString email = emailEdit->text().trimmed();
    QString n1 = newPassEdit->text();
    QString n2 = confirmPassEdit->text();

    if(curPass.isEmpty()) {
        StudioNotification::show(this, "Old password is required to verify changes.", StudioNotification::Warning);
        return;
    }

    QSqlQuery check;
    check.prepare("SELECT PASSWORD FROM USERS_SR WHERE USERNAME = :u AND PASSWORD = :p");
    check.bindValue(":u", m_username);
    check.bindValue(":p", curPass);
    if(!check.exec() || !check.next()) {
        StudioNotification::show(this, "Verification failed: Incorrect current password.", StudioNotification::Error);
        return;
    }

    if(!n1.isEmpty() && n1 != n2) {
        StudioNotification::show(this, "New password confirmation does not match.", StudioNotification::Warning);
        return;
    }

    QSqlQuery up;
    QString sql = "UPDATE USERS_SR SET EMAIL = :e";
    if(!n1.isEmpty()) sql += ", PASSWORD = :p";
    sql += " WHERE USERNAME = :u";

    up.prepare(sql);
    up.bindValue(":e", email);
    up.bindValue(":u", m_username);
    if(!n1.isEmpty()) up.bindValue(":p", n1);

    if(up.exec()) {
        StudioNotification::show(nullptr, "Profile Identity Updated", StudioNotification::Success);
        accept();
    } else {
        StudioNotification::show(this, "Update Failed: System Error.", StudioNotification::Error);
    }
}
