#ifndef BROADCASTHISTORY_H
#define BROADCASTHISTORY_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class BroadcastHistory
{
public:
    BroadcastHistory();
    BroadcastHistory(QString type, int id, QString name);
    void addEntry();
    QSqlQueryModel* display();

private:
    int showId;
    QString showName, showType;
};

#endif // BROADCASTHISTORY_H
