#include "Broadcast.h"
#include <QSqlQuery>
#include <QObject>

Broadcast::Broadcast() : id(0), duration(0), viewCount(0) {}

Broadcast::Broadcast(QString name, QString time, QString type, int duration, int viewCount)
{
    this->id = 0; // DB side
    this->name = name;
    this->startTime = time;
    this->type = type;
    this->duration = duration;
    this->viewCount = viewCount;
}

bool Broadcast::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMISSION (ID, NOM, HEURE, TYPE, DUREE, NB_VIEW) "
                  "VALUES ((SELECT NVL(MAX(ID), 0) + 1 FROM EMISSION), :nom, TO_DATE(:heure, 'YYYY-MM-DD HH24:MI'), :type, :duree, :nb_view)");
    query.bindValue(":nom", name);
    query.bindValue(":heure", startTime);
    query.bindValue(":type", type);
    query.bindValue(":duree", duration);
    query.bindValue(":nb_view", viewCount);
    return query.exec();
}

QSqlQueryModel* Broadcast::display()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    // Use TO_CHAR to format the air time nicely for the studio log
    model->setQuery("SELECT ID, NOM, TO_CHAR(HEURE, 'YYYY-MM-DD HH24:MI') as AIR_TIME, TYPE, DUREE, NB_VIEW FROM EMISSION");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SHOW NAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("AIR TIME"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DURATION (MIN)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("EST. VIEWS"));
    return model;
}

bool Broadcast::remove(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMISSION WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Broadcast::update(int id, QString name, QString time, QString type, int duration, int views)
{
    QSqlQuery query;
    query.prepare("UPDATE EMISSION SET NOM=:nom, HEURE=TO_DATE(:heure, 'YYYY-MM-DD HH24:MI'), TYPE=:type, DUREE=:duree, NB_VIEW=:nb_view WHERE ID=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", name);
    query.bindValue(":heure", time);
    query.bindValue(":type", type);
    query.bindValue(":duree", duration);
    query.bindValue(":nb_view", views);
    return query.exec();
}

QSqlQueryModel* Broadcast::sort(QString order, QString column)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMISSION ORDER BY " + column + " " + order);
    return model;
}

QSqlQueryModel* Broadcast::search(QString rech)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMISSION WHERE ID LIKE '%" + rech + "%' OR NOM LIKE '%" + rech + "%'");
    return model;
}

void Broadcast::generateStats(QVector<double>* ticks, QVector<QString> *labels)
{
    QSqlQuery q("SELECT TYPE, COUNT(*) FROM EMISSION GROUP BY TYPE");
    int i = 0;
    while (q.next()) {
        i++;
        *ticks << i;
        *labels << q.value(0).toString();
    }
}

QSqlQueryModel* Broadcast::displayCalendar(QDate x)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString day = QString::number(x.day());
    QString month = QString::number(x.month());
    QString year = QString::number(x.year());
    
    // We select the full date for the form hiddenly, but display a nice time
    QString sql = QString("SELECT ID, NOM, TO_CHAR(HEURE, 'YYYY-MM-DD HH24:MI') as FULL_TIME, "
                          "TYPE, DUREE, NB_VIEW FROM EMISSION "
                          "WHERE EXTRACT(DAY FROM HEURE)=%1 AND EXTRACT(MONTH FROM HEURE)=%2 AND EXTRACT(YEAR FROM HEURE)=%3 "
                          "ORDER BY HEURE ASC").arg(day, month, year);
    
    model->setQuery(sql);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SHOW NAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("AIR TIME"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CATEGORY"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DUR (MIN)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("EST. VIEWS"));
    return model;
}
