#ifndef GUEST_H
#define GUEST_H

#include <QString>
#include <QSqlQueryModel>
#include <QDateTime>

class Guest
{
public:
    Guest();
    Guest(int id, int reps, QDateTime dt);
    Guest(int reps, QDateTime dt);

    // Getters
    int getId() const { return id; }
    int getRepetitions() const { return repetitions; }
    QDateTime getDateTime() const { return dateTime; }

    // Setters
    void setId(int i) { id = i; }
    void setRepetitions(int r) { repetitions = r; }
    void setDateTime(QDateTime dt) { dateTime = dt; }

    // Database Actions
    bool add();
    QSqlQueryModel* display();
    bool remove(int id);
    bool update();
    QSqlQueryModel* search(QString query, int type);
    
    // Feature Methods
    QString fetchQuestion(int id);
    QString fetchAnswer(int id);
    bool checkAuth(QString username, QString password);

private:
    int id, repetitions;
    QDateTime dateTime;
};

#endif // GUEST_H
