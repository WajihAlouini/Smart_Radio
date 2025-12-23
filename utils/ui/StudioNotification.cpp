#include "StudioNotification.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QScreen>

StudioNotification::StudioNotification(QWidget* parent, const QString& message, Type type)
    : QWidget(parent), m_type(type)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);

    // Styling based on type
    QString bgColor;
    QString icon;
    switch (type) {
        case Success: bgColor = "rgba(16, 185, 129, 0.9)"; icon = "✅"; break;
        case Warning: bgColor = "rgba(245, 158, 11, 0.9)"; icon = "⚠️"; break;
        case Error:   bgColor = "rgba(239, 68, 68, 0.9)";   icon = "❌"; break;
        case Info:    bgColor = "rgba(99, 102, 241, 0.9)";  icon = "ℹ️"; break;
    }

    auto layout = new QHBoxLayout(this);
    layout->setContentsMargins(20, 10, 20, 10);
    layout->setSpacing(15);

    iconLabel = new QLabel(icon, this);
    iconLabel->setStyleSheet("font-size: 16pt;");
    
    messageLabel = new QLabel(message, this);
    messageLabel->setStyleSheet("color: white; font-weight: 700; font-family: 'Outfit'; font-size: 11pt;");

    layout->addWidget(iconLabel);
    layout->addWidget(messageLabel);

    setStyleSheet(QString("background: %1; border-radius: 12px; border: 1px solid rgba(255, 255, 255, 0.1);").arg(bgColor));

    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setXOffset(0);
    shadow->setYOffset(5);
    shadow->setColor(QColor(0, 0, 0, 80));
    setGraphicsEffect(shadow);

    adjustSize();

    displayTimer = new QTimer(this);
    displayTimer->setSingleShot(true);
    connect(displayTimer, &QTimer::timeout, [this]() {
        QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
        anim->setDuration(500);
        anim->setStartValue(1.0);
        anim->setEndValue(0.0);
        connect(anim, &QPropertyAnimation::finished, this, &QWidget::close);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
    });
}

void StudioNotification::show(QWidget* parent, const QString& message, Type type)
{
    StudioNotification* toast = new StudioNotification(parent, message, type);
    
    // Position: Bottom Right of the parent or screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = screenGeometry.width() - toast->width() - 40;
    int y = screenGeometry.height() - toast->height() - 80;
    
    toast->move(x, y);
    toast->startAnimation();
}

void StudioNotification::startAnimation()
{
    setWindowOpacity(0.0);
    QWidget::show();
    
    QPropertyAnimation* anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(400);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    
    displayTimer->start(3000); // Visible for 3 seconds
}
