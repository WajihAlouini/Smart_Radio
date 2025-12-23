#ifndef STUDIONOTIFICATION_H
#define STUDIONOTIFICATION_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QTimer>
#include <QGraphicsDropShadowEffect>

class StudioNotification : public QWidget {
    Q_OBJECT
public:
    enum Type { Success, Warning, Error, Info };

    static void show(QWidget* parent, const QString& message, Type type = Success);

private:
    explicit StudioNotification(QWidget* parent, const QString& message, Type type);
    void startAnimation();

    QLabel* iconLabel;
    QLabel* messageLabel;
    QTimer* displayTimer;
    Type m_type;
};

#endif // STUDIONOTIFICATION_H
