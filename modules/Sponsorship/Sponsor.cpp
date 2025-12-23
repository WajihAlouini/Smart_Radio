#include "Sponsor.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtDebug>
#include <QSqlError>
#include <utility> // For std::move

Sponsor::Sponsor() : id(0), name(""), duration(""), airtime(""), repetitions(0) {}

Sponsor::Sponsor(int id, QString n, QString d, QString a, int r) : id(id), name(n), duration(d), airtime(a), repetitions(r) {}

Sponsor::Sponsor(QString n, QString d, QString a, int r) : id(0), name(n), duration(d), airtime(a), repetitions(r) {}

bool Sponsor::add()
{
    QSqlQuery query;
    // Ensure column exists and backfill any NULL IDs for existing data
    query.exec("ALTER TABLE sponsor ADD id NUMBER");
    query.exec("UPDATE sponsor SET id = ROWNUM WHERE id IS NULL");
    
    query.prepare("INSERT INTO sponsor (id, nom, duree, heure, repetition) "
                  "VALUES ((SELECT NVL(MAX(id), 0) + 1 FROM sponsor), :nom, :duree, :heure, :repetition)");
    query.bindValue(":nom", name);
    query.bindValue(":duree", duration);
    query.bindValue(":heure", airtime);
    query.bindValue(":repetition", repetitions);
    return query.exec();
}

bool Sponsor::remove(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM sponsor WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Sponsor::update(int id)
{
    QSqlQuery q;
    q.prepare("UPDATE sponsor SET nom=:nom, heure=:heure, duree=:duree, repetition=:repetition WHERE id=:id");
    q.bindValue(":id", id);
    q.bindValue(":nom", name);
    q.bindValue(":heure", airtime);
    q.bindValue(":duree", duration);
    q.bindValue(":repetition", repetitions);
    return q.exec();
}

QSqlQueryModel* Sponsor::display()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id, nom, duree, heure, repetition FROM sponsor ORDER BY id DESC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PARTNER NAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DURATION (S)"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("AIRTIME"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("REPETITIONS"));
    return model;
}

QSqlQueryModel* Sponsor::sort()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT id, nom, duree, heure, repetition FROM sponsor ORDER BY nom ASC");
    return model;
}

QSqlQueryModel* Sponsor::search(QString query)
{
    QSqlQuery q;
    QSqlQueryModel *model = new QSqlQueryModel();
    q.prepare("SELECT id, nom, duree, heure, repetition FROM sponsor WHERE nom LIKE :q OR TO_CHAR(id) = :idQ");
    q.bindValue(":q", "%" + query + "%");
    q.bindValue(":idQ", query);
    q.exec();
    model->setQuery(std::move(q)); // Use move to follow Qt 6 best practices
    return model;
}
