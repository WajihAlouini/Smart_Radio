#ifndef SPONSOR_H
#define SPONSOR_H

#include <QString>
#include <QSqlQueryModel>
#include <QTableView>

class Sponsor
{
public:
    Sponsor();
    Sponsor(int id, QString name, QString duration, QString airtime, int repetitions);
    Sponsor(QString name, QString duration, QString airtime, int repetitions);

    // Accessors
    int getId() const { return id; }
    QString getName() const { return name; }
    QString getDuration() const { return duration; }
    QString getAirtime() const { return airtime; }
    int getRepetitions() const { return repetitions; }

    void setId(int i) { id = i; }
    void setName(QString n) { name = n; }
    void setDuration(QString d) { duration = d; }
    void setAirtime(QString a) { airtime = a; }
    void setRepetitions(int r) { repetitions = r; }

    // Database Actions
    bool add();
    bool remove(int id);
    bool update(int id);
    QSqlQueryModel* display();
    QSqlQueryModel* sort();
    QSqlQueryModel* search(QString query);

private:
    int id;
    QString name, duration, airtime;
    int repetitions;
};

#endif // SPONSOR_H
