#ifndef STAFF_H
#define STAFF_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>

class Staff
{
private:
    int id, salary;
    QString firstName, lastName, position;

public:
    Staff();
    Staff(QString, QString, QString, int);

    // Getters
    int getId() const { return id; }
    int getSalary() const { return salary; }
    QString getFirstName() const { return firstName; }
    QString getLastName() const { return lastName; }
    QString getPosition() const { return position; }

    // Setters
    void setId(int i) { id = i; }
    void setSalary(int s) { salary = s; }
    void setFirstName(QString fn) { firstName = fn; }
    void setLastName(QString ln) { lastName = ln; }
    void setPosition(QString p) { position = p; }

    // Database Actions
    bool add();
    bool remove(int id);
    QSqlQueryModel* display();
    bool update(QString id, QString firstName, QString lastName, QString position, QString salary);
    QSqlQueryModel* sort();
    void search(QString query, QTableView *view);
};

#endif // STAFF_H
