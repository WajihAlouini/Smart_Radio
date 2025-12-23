#ifndef BROADCAST_H
#define BROADCAST_H

#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QDate>
#include <QVector>

class Broadcast
{
public:
    Broadcast();
    Broadcast(QString, QString, QString, int, int);

    // Business Logic
    bool add();
    QSqlQueryModel* display();
    bool remove(int id);
    bool update(int id, QString name, QString time, QString type, int duration, int views);
    QSqlQueryModel* sort(QString column, QString order);
    QSqlQueryModel* search(QString query);
    void generateStats(QVector<double>* ticks, QVector<QString> *labels);
    QSqlQueryModel* displayCalendar(QDate date);

private:
    int id;
    QString name;
    QString startTime;
    QString type;
    int duration;
    int viewCount;
};

#endif // BROADCAST_H
