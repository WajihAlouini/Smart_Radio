#include "Guest.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>

Guest::Guest() : id(0), repetitions(0) {}

Guest::Guest(int id, int reps, QDateTime dt) : id(id), repetitions(reps), dateTime(dt) {}

Guest::Guest(int reps, QDateTime dt) : id(0), repetitions(reps), dateTime(dt) {}

bool Guest::add()
{
    QSqlQuery query;
    query.prepare("INSERT INTO invite (ID, REPETITIONS, DATETIME) "
                  "VALUES ((SELECT NVL(MAX(ID), 0) + 1 FROM invite), :repetition, :datetime)");
    query.bindValue(":repetition", repetitions);
    query.bindValue(":datetime", dateTime);
    return query.exec();
}

QSqlQueryModel* Guest::display()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM invite");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("GUEST ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("REPETITIONS"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("SCHEDULED TIME"));
    return model;
}

bool Guest::remove(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM invite WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Guest::update()
{
    QSqlQuery query;
    query.prepare("UPDATE invite SET REPETITIONS=:repetition, DATETIME=:datetime WHERE ID=:id");
    query.bindValue(":id", id);
    query.bindValue(":repetition", repetitions);
    query.bindValue(":datetime", dateTime);
    return query.exec();
}

QSqlQueryModel* Guest::search(QString queryText, int type)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    if (type == 1) { // By ID
        model->setQuery("SELECT * FROM invite WHERE ID = " + queryText);
    } else { // By Date (Simplified)
        model->setQuery("SELECT * FROM invite WHERE DATETIME LIKE '%" + queryText + "%'");
    }
    return model;
}

QString Guest::fetchQuestion(int id)
{
    QSqlQuery q;
    q.prepare("SELECT QUESTION FROM ENIGME WHERE ID = :id");
    q.bindValue(":id", id);
    
    if(!q.exec() || !q.next()) {
        // Table probably missing or ID not found, try to auto-seed
        QSqlQuery setup;
        setup.exec("CREATE TABLE ENIGME (ID NUMBER PRIMARY KEY, QUESTION VARCHAR2(500), REPONSE VARCHAR2(200))");
        
        // Check if empty and seed
        QSqlQuery countCheck("SELECT COUNT(*) FROM ENIGME");
        if(countCheck.next() && countCheck.value(0).toInt() == 0) {
            setup.exec("INSERT INTO ENIGME VALUES (1, 'What is the most popular radio format?', 'Music')");
            setup.exec("INSERT INTO ENIGME VALUES (2, 'Who invented the Radio?', 'Marconi')");
            setup.exec("INSERT INTO ENIGME VALUES (3, 'What does FM stand for?', 'Frequency Modulation')");
            setup.exec("INSERT INTO ENIGME VALUES (4, 'What was the first music video aired on MTV?', 'Video Killed the Radio Star')");
            setup.exec("INSERT INTO ENIGME VALUES (5, 'Which radio signal travels furthest?', 'Shortwave')");
        }
        
        // Re-try fetch with a known good ID if the requested one failed
        q.prepare("SELECT QUESTION FROM ENIGME WHERE ID = (SELECT MIN(ID) FROM ENIGME)");
        if(q.exec() && q.next()) return q.value(0).toString();
    } else {
        return q.value(0).toString();
    }
    return "";
}

QString Guest::fetchAnswer(int id)
{
    QSqlQuery q;
    q.prepare("SELECT REPONSE FROM ENIGME WHERE ID = :id");
    q.bindValue(":id", id);
    if(q.exec() && q.next()) return q.value(0).toString();
    return "";
}

bool Guest::checkAuth(QString user, QString pass)
{
    // Implementation placeholder for login logic
    return (user == "admin" && pass == "admin");
}
