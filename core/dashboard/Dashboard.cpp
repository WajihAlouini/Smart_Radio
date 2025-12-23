#include "Dashboard.h"
#include "ui_Dashboard.h"
#include <QShortcut>
#include <QKeySequence>
#include <QDateTime>
#include <QSqlQuery>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "ThemeManager.h"
#include "Logger.h"
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

Dashboard::Dashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Dashboard)
{
    ui->setupUi(this);
    
    // Apply Global Performance Theme
    this->setStyleSheet(ThemeManager::instance().getGlobalStyleSheet());
    this->setWindowTitle("SmartRadio Studio Engine v3.0 [PREMIUM]");
    this->setFixedSize(1200, 800); 

    setupEnhancedUI();

    // Start Real-Time Core
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &Dashboard::updateDateTime);
    connect(updateTimer, &QTimer::timeout, this, &Dashboard::refreshStats);
    updateTimer->start(1000);

    updateDateTime();
    refreshStats();

    // --- Keyboard Shortcuts ---
    new QShortcut(QKeySequence("Ctrl+1"), this, SLOT(on_pushButton_clicked()));
    new QShortcut(QKeySequence("Ctrl+2"), this, SLOT(on_pushButton_2_clicked()));
    new QShortcut(QKeySequence("Ctrl+3"), this, SLOT(on_pushButton_3_clicked()));
    new QShortcut(QKeySequence("Ctrl+4"), this, SLOT(on_pushButton_4_clicked()));
    new QShortcut(QKeySequence("Ctrl+Q"), this, SLOT(close()));
    
    LOG_INFO("Dashboard Core UI initialized", "Dashboard");
}

#include <QApplication>
#include <QProcess>
#include "StudioNotification.h"

void Dashboard::setCurrentUser(const QString& user) {
    currentUser = user;
    if (userLabel) userLabel->setText("👤 " + user.toUpper());
}

void Dashboard::setupEnhancedUI() {
    // 1. Sidebar Branding
    ui->label->setObjectName("SidebarContainer");
    ui->label->setGeometry(0, 0, 280, 800);
    
    ui->label_2->setText("📡\nSMART\nRADIO");
    ui->label_2->setGeometry(20, 40, 240, 150);
    ui->label_2->setStyleSheet("color: #6366f1; font-size: 26pt; font-weight: 950; letter-spacing: 2px; line-height: 0.8;");
    ui->label_2->setAlignment(Qt::AlignCenter);

    // 2. Identity & Top Bar
    userLabel = new QLabel("👤 GUEST", this);
    userLabel->setGeometry(850, 30, 200, 30);
    userLabel->setStyleSheet("color: #94a3b8; font-weight: 800; font-size: 10pt; letter-spacing: 1px;");
    userLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    logoutBtn = new QPushButton("SIGN OUT", this);
    logoutBtn->setGeometry(1060, 25, 100, 40);
    logoutBtn->setStyleSheet(
        "QPushButton { background: rgba(239, 68, 68, 0.1); color: #ef4444; border: 1px solid #ef4444; border-radius: 8px; font-size: 8pt; }"
        "QPushButton:hover { background: #ef4444; color: white; }"
    );
    connect(logoutBtn, &QPushButton::clicked, this, &Dashboard::on_logoutBtn_clicked);

    QPushButton* editProfileBtn = new QPushButton("EDIT PROFILE", this);
    editProfileBtn->setGeometry(730, 25, 110, 40);
    editProfileBtn->setStyleSheet(
        "QPushButton { background: rgba(99, 102, 241, 0.1); color: #6366f1; border: 1px solid #6366f1; border-radius: 8px; font-size: 8pt; }"
        "QPushButton:hover { background: #6366f1; color: white; }"
    );
    connect(editProfileBtn, &QPushButton::clicked, this, &Dashboard::on_profileBtn_clicked);

    // 3. Studio Clock (Digital)
    clockLabel = new QLabel(this);
    clockLabel->setObjectName("LiveClock");
    clockLabel->setGeometry(320, 30, 400, 60);
    clockLabel->setText("00:00:00");

    statusIndicator = new QLabel("● STUDIO ONLINE", this);
    statusIndicator->setGeometry(320, 90, 200, 30);
    statusIndicator->setStyleSheet("color: #10b981; font-weight: 800; font-size: 10pt;");

    // 4. Stats Section
    int statsX = 320;
    int statsY = 150;
    int cardW = 260;
    int cardH = 120;
    int gap = 25;

    auto createStatsCard = [&](int x, int y, QString title, QLabel*& valueLabel) {
        QFrame* card = new QFrame(this);
        card->setObjectName("StatsCard");
        card->setGeometry(x, y, cardW, cardH);
        ThemeManager::instance().applyShadow(card);

        QLabel* t = new QLabel(title, card);
        t->setObjectName("StatsTitle");
        t->setGeometry(20, 15, 200, 20);

        valueLabel = new QLabel("0", card);
        valueLabel->setObjectName("StatsValue");
        valueLabel->setGeometry(20, 45, 200, 50);
    };

    createStatsCard(statsX, statsY, "TOTAL EMPLOYEES", staffCountLabel);
    createStatsCard(statsX + cardW + gap, statsY, "LIVE BROADCASTS", broadcastCountLabel);
    createStatsCard(statsX + (cardW + gap) * 2, statsY, "ACTIVE SPONSORS", sponsorCountLabel);

    // 5. Module Navigation Buttons
    QString btnStyle = 
        "QPushButton { "
        "  background: rgba(30, 41, 59, 0.4); "
        "  border: 1px solid rgba(255, 255, 255, 0.1); border-left: 6px solid #6366f1; border-radius: 12px; "
        "  padding: 25px; text-align: left; font-size: 14pt; color: #f1f5f9; font-weight: 700; "
        "} "
        "QPushButton:hover { "
        "  background-color: rgba(99, 102, 241, 0.1); border-color: #818cf8; color: #ffffff; "
        "  border-left: 10px solid #818cf8; "
        "}";

    ui->pushButton->setStyleSheet(btnStyle);
    ui->pushButton_2->setStyleSheet(btnStyle);
    ui->pushButton_3->setStyleSheet(btnStyle);
    ui->pushButton_4->setStyleSheet(btnStyle);

    int btnX = 320;
    int btnY = 320;
    int btnW = 830;
    int btnH = 90;
    int btnGap = 20;

    ui->pushButton->setGeometry(btnX, btnY, btnW, btnH);
    ui->pushButton_2->setGeometry(btnX, btnY + (btnH + btnGap), btnW, btnH);
    ui->pushButton_3->setGeometry(btnX, btnY + (btnH + btnGap) * 2, btnW, btnH);
    ui->pushButton_4->setGeometry(btnX, btnY + (btnH + btnGap) * 3, btnW, btnH);

    ui->pushButton->setText("  👥  STAFF DIRECTORY - HR & Payroll");
    ui->pushButton_2->setText("  📻  BROADCAST HUB - Live Operations");
    ui->pushButton_3->setText("  🎙️  GUEST MANAGEMENT - Talent Booking");
    ui->pushButton_4->setText("  💎  SPONSORSHIP - Revenue Control");

    // --- Entrance Animations ---
    QParallelAnimationGroup* entranceGroup = new QParallelAnimationGroup(this);
    auto animateCard = [&](QLabel* label) {
        QGraphicsOpacityEffect* opacity = new QGraphicsOpacityEffect(label->parentWidget());
        label->parentWidget()->setGraphicsEffect(opacity);
        QPropertyAnimation* anim = new QPropertyAnimation(opacity, "opacity");
        anim->setDuration(1200);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->setEasingCurve(QEasingCurve::OutCubic);
        entranceGroup->addAnimation(anim);
    };
    animateCard(staffCountLabel);
    animateCard(broadcastCountLabel);
    animateCard(sponsorCountLabel);
    entranceGroup->start(QAbstractAnimation::DeleteWhenStopped);

    // --- Status Pulsing ---
    QGraphicsOpacityEffect* statusOpacity = new QGraphicsOpacityEffect(statusIndicator);
    statusIndicator->setGraphicsEffect(statusOpacity);
    QPropertyAnimation* pulseStatus = new QPropertyAnimation(statusOpacity, "opacity");
    pulseStatus->setDuration(1500);
    pulseStatus->setStartValue(1.0);
    pulseStatus->setEndValue(0.4);
    pulseStatus->setLoopCount(-1);
    pulseStatus->setEasingCurve(QEasingCurve::InOutSine);
    pulseStatus->start();
    
    StudioNotification::show(this, "Studio Engine Optimized & Ready", StudioNotification::Info);
}

#include "ProfileDialog.h"

void Dashboard::on_profileBtn_clicked() {
    ProfileDialog diag(currentUser, this);
    diag.exec();
}

void Dashboard::on_logoutBtn_clicked() {
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void Dashboard::updateDateTime() {
    clockLabel->setText(QDateTime::currentDateTime().toString("HH:mm:ss"));
}

void Dashboard::refreshStats() {
    QSqlQuery query;
    
    // Correct table names for SmartRadio Database
    if (query.exec("SELECT COUNT(*) FROM PERSONNEL") && query.next())
        staffCountLabel->setText(query.value(0).toString());
        
    if (query.exec("SELECT COUNT(*) FROM EMISSION") && query.next())
        broadcastCountLabel->setText(query.value(0).toString());
        
    if (query.exec("SELECT COUNT(*) FROM SPONSOR") && query.next())
        sponsorCountLabel->setText(query.value(0).toString());
}

Dashboard::~Dashboard()
{
    delete ui;
}

void Dashboard::on_pushButton_clicked()
{
    staffModule = new StaffModule(this);
    staffModule->show();
}

void Dashboard::on_pushButton_2_clicked()
{
    broadcastModule = new BroadcastModule(this);
    broadcastModule->show();
}

void Dashboard::on_pushButton_3_clicked()
{
    guestModule = new GuestModule(this);
    guestModule->show();
}

void Dashboard::on_pushButton_4_clicked()
{
    sponsorshipModule = new SponsorshipModule(this);
    sponsorshipModule->show();
}
