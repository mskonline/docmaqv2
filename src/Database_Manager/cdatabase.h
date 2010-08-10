/* DocmaQ v2.0, Credential Publishing System
    Copyright (C) 2010 M.Sai Kumar <msk.mymails@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef CDATABASE_H
#define CDATABASE_H

#include <QtGui>
#include <QtSql>
#include "../Data_Structures/student.h"

class CDatabase :public QObject
{
    Q_OBJECT

public:
    QString qp1,qp2,qp3,qp4,qp5,qpc;
    QString gender,year,sem,username,user_id;
    QString dbstatus;
    QDate d_b,d_a;
    bool is_connected;

    CDatabase();
    void connect(QStringList dbdetails);
    bool reconnect(QStringList dbdetails);
    void format_course(Student *);
    void format_tc(Student *);

    bool fetch(Student *);
    bool fetch_ex(Student *);

    int check_duplicate_issue(Student *);
    int update_dissue(Student *);
    int log_tc(Student *);
    ~CDatabase();

private:
    Student *student;
    int cmode;

signals:
    void user_passwd(QString);
    void report(int,QString);

public slots:
    void get_user_password(QString);
};

#endif // CDATABASE_H
