#ifndef CDATABASE_H
#define CDATABASE_H

#include <QtGui>
#include <QtSql>
#include "../Data_Structures/student.h"

class CDatabase :public QObject
{
    Q_OBJECT
private:
   Student *student;

   int cmode;
public:
    CDatabase();
    QSqlDatabase db;
    QString qp1,qp2,qp3,qp4,qp5;
    QString gender,year,sem,username;

    QDate d_b,d_a;
    void format_course(Student *);
    bool connect();//QStringList dbdetails);
    bool reconnect(QStringList dbdetails);
    bool fetch(int ,Student *);
    bool fetch_ex();

public slots:
    void get_user_password(QString);

signals:
    void user_passwd(QString);
};

#endif // CDATABASE_H
