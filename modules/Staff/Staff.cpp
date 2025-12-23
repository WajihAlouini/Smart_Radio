#include "Staff.h"
#include <QSqlQuery>
#include <QtDebug>

Staff::Staff()
{
    id = 0;
    firstName = "";
    lastName = "";
    position = "";
    salary = 0;
}

Staff::Staff(QString firstName, QString lastName, QString position, int salary)
{
    this->id = 0; // Handled by DB
    this->firstName = firstName;
    this->lastName = lastName;
    this->position = position;
    this->salary = salary;
}

bool Staff::add()
{
    QSqlQuery query;
    // Uses Oracle's NVL and MAX to simulate an auto-increment identity
    query.prepare("INSERT INTO PERSONNEL (ID, NOMP, PRENOM, FONCTION, SALAIRE) "
                  "VALUES ((SELECT NVL(MAX(ID), 0) + 1 FROM PERSONNEL), :nomp, :prenom, :fonction, :salaire)");

    query.bindValue(":nomp", firstName);
    query.bindValue(":prenom", lastName);
    query.bindValue(":fonction", position);
    query.bindValue(":salaire", salary);
    return query.exec();
}

bool Staff::remove(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PERSONNEL WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* Staff::display()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PERSONNEL");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("FIRST NAME"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("LAST NAME"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("POSITION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("SALARY"));
    return model;
}

bool Staff::update(QString id, QString firstName, QString lastName, QString position, QString salary)
{
    QSqlQuery query;
    query.prepare("UPDATE PERSONNEL SET NOMP=:nomp, PRENOM=:prenom, FONCTION=:fonction, SALAIRE=:salaire WHERE ID=:id");
    query.bindValue(":id", id);
    query.bindValue(":nomp", firstName);
    query.bindValue(":prenom", lastName);
    query.bindValue(":fonction", position);
    query.bindValue(":salaire", salary);
    return query.exec();
}

void Staff::search(QString name, QTableView *view)
{
    QSqlQuery query;
    QSqlQueryModel *model = new QSqlQueryModel();
    query.prepare("SELECT * FROM PERSONNEL WHERE NOMP LIKE :name");
    query.bindValue(":name", "%" + name + "%");
    query.exec();
    model->setQuery(query);
    view->setModel(model);
}

QSqlQueryModel* Staff::sort()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM PERSONNEL ORDER BY NOMP ASC");
    return model;
}
