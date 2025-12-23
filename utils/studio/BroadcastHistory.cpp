#include "BroadcastHistory.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QObject>

BroadcastHistory::BroadcastHistory() : showId(0) {}

BroadcastHistory::BroadcastHistory(QString type, int id, QString name)
{
    this->showType = type;
    this->showId = id;
    this->showName = name;
}

void BroadcastHistory::addEntry()
{
    QSqlQuery query;
    query.prepare("INSERT INTO HISTORIQUE (TYPE, ID, NOM) VALUES (:type, :id, :nom)");
    query.bindValue(":type", showType);
    query.bindValue(":id", showId);
    query.bindValue(":nom", showName);
    if(!query.exec()) {
        // Table might be missing (e.g., ORA-00942), try to create it
        QSqlQuery createQuery;
        createQuery.exec("CREATE TABLE HISTORIQUE (TYPE VARCHAR(50), ID NUMBER, NOM VARCHAR(100))");
        // Retry insertion
        query.exec();
    }
}

QSqlQueryModel* BroadcastHistory::display()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM HISTORIQUE");
    
    // Check for errors (like table not found)
    if (model->lastError().isValid()) {
        QSqlQuery createQuery;
        if (createQuery.exec("CREATE TABLE HISTORIQUE (TYPE VARCHAR(50), ID NUMBER, NOM VARCHAR(100))")) {
            model->setQuery("SELECT * FROM HISTORIQUE");
        }
    }
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SHOW ID"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("SHOW NAME"));
    return model;
}
