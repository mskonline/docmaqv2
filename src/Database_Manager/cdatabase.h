#ifndef CDATABASE_H
#define CDATABASE_H

#include <QtGui>
#include <QtSql>
#include "../Data_Structures/student.h"

class CDatabase :public QObject
{
    Q_OBJECT

public:
    QSqlDatabase db;
    QString qp1,qp2,qp3,qp4,qp5,qpc;
    QString gender,year,sem,username;
    QDate d_b,d_a;
    bool is_connected;

    CDatabase();
    void format_course(Student *);
    bool connect();//QStringList dbdetails);
    bool reconnect(QStringList dbdetails);
    bool fetch(Student *);
    int check_duplicate_issue(Student *);
    int update_dissue(Student *);
    int log_tc(Student * );
    bool fetch_ex(Student *);

private:
    Student *student;
    int cmode;

public slots:
    void get_user_password(QString);

signals:
    void user_passwd(QString);
    void report(int,QString);
};

#endif // CDATABASE_H
