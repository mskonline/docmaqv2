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

#include "cdatabase.h"
#include <QString>

CDatabase::CDatabase()
{
    qp1 = "select student_id from student_admission where student_rollno=";
    qp2 = "select student_name,father_name,gender from student_info where student_id=" ;
    qp3 = "select course,branch_id,current_sem,current_year from student_academic where student_id=" ;
    qp4 = "select date_of_birth,caste from student_info where student_id=";
    qp5 = "select date_of_adm from student_admission where student_id=" ;
    qpc = "select duplicates from tc_log where student_id=";

    dbstatus = "    Manual Mode";
    is_connected = false;
}

void CDatabase::connect(QStringList dbinfo)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","docmaq");
    db.setDatabaseName(dbinfo.at(0));
    db.setHostName(dbinfo.at(1));
    db.setPort(dbinfo.at(2).toInt());
    db.setUserName(dbinfo.at(3));
    db.setPassword(dbinfo.at(4));

    is_connected = db.open();

    if(db.lastError().isValid())
        dbstatus = db.lastError().databaseText();
    else
        dbstatus = "    Connected to Database";
}

bool CDatabase::reconnect(QStringList dbdetails)
{
    if(QSqlDatabase::contains("docmaq"))
        QSqlDatabase::removeDatabase("docmaq");

    connect(dbdetails);
    return is_connected;
}

/* bool fetch(Student *)
 * fetches details for Bonafide & Conduct
 */
bool CDatabase::fetch(Student *student)
{
    QString q;
    QSqlQuery query(QSqlDatabase::database("docmaq"));

    if(!student->st_filled)
    {
        // first query,fetches students unique ID
        q = qp1 + "'" + student->roll  + "'" ;
        query.exec(q);
        query.next();
        student->id = query.value(0).toString();

        if( student->id.isEmpty() )
        {
            emit report(ERROR,"No Details Found for : " + student->roll + " . Check whether you have entered the Roll Number Correctly or Whether the Database is Configured Correctly.\n\n" +
                        "MySQL Reports : " + query.lastError().databaseText()) ;
            return false;
        }

        // second query,fetches student name,fathers name, gender
        q = qp2 + student->id;
        query.exec(q);
        query.next();
        student->name = query.value(0).toString().toUpper();
        student->father_name = query.value(1).toString().toUpper();
        gender = query.value(2).toString();

        if ( student->name.isEmpty() && student->father_name.isEmpty() && gender.isEmpty() )
            emit report(WARNING , "Few Empty Details in Database for : " + student->roll + ". Please Fill them Manually.");

        // third query,fetches course details
        q = qp3 + student->id;
        query.exec(q);
        query.next();
        student->course = query.value(0).toString();
        student->bid = query.value(1).toInt();
        sem = query.value(2).toString();
        year = query.value(3).toString();

        if ( student->course.isEmpty() &&  student->bid == 0 && year.isEmpty() && sem.isEmpty())
            emit report(WARNING , "Few Empty Details in Database for : " + student->roll + ". Please Fill them Manually.");

        student->st_filled = 1;
    }

    return true;
}

bool CDatabase::fetch_ex(Student *student)
{
    QSqlQuery query(QSqlDatabase::database("docmaq"));

    if(fetch(student))
    {
        QString c,q;

        if(check_duplicate_issue(student))
            return false;

        q = qp4 + student->id;
        query.exec(q);
        query.next();

        d_b = query.value(0).toDate();
        c = query.value(1).toString();

        q = qp5 + student->id;
        query.exec(q);
        query.next();

        d_a = query.value(0).toDate();

        student->std_ex->dob = d_b.toString("dd.MMM.yyyy");
        student->std_ex->doa = d_a.toString("dd.MMM.yyyy");

        if(student->std_ex->is)
        {
            QDate dt;
            dt = QDate().fromString(student->std_ex->dissue->at(1),"yyyy-MM-dd");
            student->std_ex->type = "DUPLICATE";
            student->std_ex->dol = dt.toString("dd.MMM.yyyy");
        }
        else
        {
            student->std_ex->type = "ORIGINAL";
            student->std_ex->dol = QDate::currentDate().toString("dd.MMM.yyyy");
        }

        student->std_ex->qualified = "Yes";
        student->std_ex->dues = "No";
        student->std_ex->disp_m = "None";
        student->std_ex->conduct = "Satisfactory";
        student->std_ex->remarks = "";

        if(c != "OC")
            student->std_ex->community = c;
        else
            student->std_ex->community = "None";

        student->stx_filled = 1;
    }
    return true;
}

int CDatabase::check_duplicate_issue(Student *student)
{
    QSqlQuery query(QSqlDatabase::database("docmaq"));
    QString q;

    q = qpc + student->id;
    query.exec(q);

    if(query.lastError().type() != QSqlError::NoError)
    {
        emit report(ERROR,"An Error occurred while checking for any previous issues of TC for : " + student->roll + " . Please check if the Database is properly configured.\n\n" +
                    "MySQL Reports : " + query.lastError().databaseText()) ;
        return 1;
    }

    query.next();

    if(!query.value(0).toString().isEmpty())
    {
        QString q = "Select duplicates, org_issued_on, org_issued_by,dup_issued_on, dup_issued_by, remarks from tc_log where student_id=" + student->id;
        query.exec(q);

        query.next();

        student->std_ex->dissue->clear();
        student->std_ex->dissue->append(query.value(0).toString()); // duplicates
        student->std_ex->dissue->append(query.value(1).toString()); // org_issued_on
        student->std_ex->dissue->append(query.value(2).toString()); // org_issued_by
        student->std_ex->dissue->append(query.value(3).toString()); // dup_issued_on
        student->std_ex->dissue->append(query.value(4).toString()); // dup_issued_by
        student->std_ex->dissue->append(query.value(5).toString()); // Remarks

        student->std_ex->is = 1;
    }
    else
        student->std_ex->is = 0;

    return 0;
}

int CDatabase::update_dissue(Student *student)
{
    QString q ;
    QSqlQuery query(QSqlDatabase::database("docmaq"));

    QString d,i,r ;

    d = "'" + student->std_ex->dissue->at(3) + "'";
    i = "'" + student->std_ex->dissue->at(4) + "'";
    r = "'" + student->std_ex->dissue->at(5) + "'";


    q = "update tc_log set duplicates =" + student->std_ex->dissue->at(0) + "," +tr(" dup_issued_on=") +  d +  "," + tr(" dup_issued_by=") +
        i  +  "," + tr(" remarks=") + r + " where student_id=" + student->id;

    query.exec(q);
    if(query.lastError().type() != QSqlError::NoError)
    {
        emit report(ERROR,"An Error occurred while updating Remarks of TC for : " + student->roll + " . Please check if the Database is properly configured\n\n" +
                    "MySQL Reports : " + query.lastError().databaseText()) ;
        return -1;
    }

    return 0;
}

/* void log_tc(QStringList)
 * Called after Printing is completed & is logged in the TC Log
 * Record is created when TC is issued for the FIRST TIME
 */
int CDatabase::log_tc(Student *student)
{
    QSqlQuery query(QSqlDatabase::database("docmaq"));
    QString q,d,i;

    d = "'"+ QDate::currentDate().toString("yyyy-MM-dd") + "'"; // Date format : yyyy-MM-dd
    i = "'" + username + "'";

    q = "insert into tc_log(student_id,duplicates,org_issued_on,org_issued_by) values(" + student->id + "," +
        tr("0") + "," +  d +  "," + i + ")";

    query.exec(q);

    if(query.lastError().type() != QSqlError::NoError)
    {
        report (ERROR,"An Error occurred while Recording an issue of TC for : " + student->roll + " . Please check if the Database is properly configured \n\n" +
                "MySQL Reports : " + query.lastError().databaseText()) ;
        return -1;
    }

    return 0;
}

/* void get_user_password(QString)
 * Called by AuthenDialog to obtain User's Password
 */
void CDatabase::get_user_password(QString userid)
{
    QString q1,q2,passwd;
    QSqlQuery query(QSqlDatabase::database("docmaq"));
    QSqlQuery query2(QSqlDatabase::database("docmaq"));

    q1 = "select passwd from login_info where user_name='" + userid + "' and role='academic'" ;

    query.exec(q1);
    query.next();

    passwd = query.value(0).toString();

    if(!passwd.isEmpty())
    {
        q2 = "select teacher_name from teacher_info where teacher_code='" + userid + "'";
        query2.exec(q2);
        query2.next();
        this->user_id = userid;
        username = query2.value(0).toString();
    }

    emit user_passwd(passwd);
}

void CDatabase::format_course(Student *student)
{
    int var_yr = year.toInt();

    // Year
    switch ( var_yr )
    {
    case 1: year = ", I year";
        break;
    case 2: year = ", II year";
        break;
    case 3: year = ", III year";
        break;
    case 4: year = ", IV year";
    }

    int var_sem = sem.toInt();

    // B'tech 1st year don't have semester system
    if (student->course == "B.Tech" and var_yr == 1)
    {
        sem = "";
        var_sem = 0;
    }
    else
    {
        switch ( var_sem )
        {
        case 1 : sem = ", I semester";
            break;
        case 2 : sem = ", II semester";
        }
    }

    // Branch
    switch ( student->bid )
    {
    case 1 : student->branch ="EEE";
        break;
    case 2 : student->branch ="CSE";
        break;
    case 3 : student->branch ="ECE";
        break;
    case 4 : student->branch ="IT";
        break;
    case 5 : student->branch ="Mech";
        break;
    case 6 : student->branch ="Bio - Tech";
        break;
    case 9 : student->branch ="ECM";
        break;
    case 10 : student->branch ="S & H";
        break;
    case 12 : student->branch ="";//MBA
        break;
    case 13 : student->branch ="AM";
        break;
    case 14 : student->branch ="EET";
        break;
    case 15 : student->branch ="EPE";
        break;
    case 16 : student->branch ="DSCE";
        break;
    case 17 : student->branch ="VLSI";
        break;
    case 19 : student->branch ="SE";
        break;
    case 20 : student->branch ="CAD - CAM";
        break;
    case 21 : student->branch ="Bio-Tech";
        break;
    case 22 : student->branch ="";//MCA
        break;
    default : student->branch = "";
}

    QString temp2 = student->branch;
    temp2 = !temp2.isEmpty() ? " - " + temp2 : "";

    // constructing course details.
    student->cdetails = student->course + temp2 + year + sem;

    if(gender == "Male")
    {
        student->name = "Mr. " + student->name;
        student->co = "S/o";
    }
    else
    {
        student->name = "Ms. " + student->name;
        student->co = "D/o";
    }
}

void CDatabase::format_tc(Student *student)
{
    switch ( student->bid )
    {
    case 1 : student->branch ="Electricals and Electronics Engineering";
        break;
    case 2 : student->branch ="Computer Science and Engineering";
        break;
    case 3 : student->branch ="Electronics and Communication Engineering";
        break;
    case 4 : student->branch ="Information Technology";
        break;
    case 5 : student->branch ="Mechanical Engineering";
        break;
    case 6 : student->branch ="Bio Tech Engineering";
        break;
    case 9 : student->branch ="Electronics and Computer Engineering";
        break;
    case 10 : student->branch ="Science & Humanities";
        break;
    case 12 : student->branch ="Master of Business Adminstration";//MBA
        break;
    case 13 : student->branch ="Airline Management";
        break;
    case 14 : student->branch ="Electronic Engineering Technology";
        break;
    case 15 : student->branch ="Electrical and Power Engineering";
        break;
    case 16 : student->branch ="Digital Systems and Computer Electronics";
        break;
    case 17 : student->branch ="VLSI and Embedded Systems";
        break;
    case 19 : student->branch ="Software Engineering";
        break;
    case 20 : student->branch ="CAD - CAM";
        break;
    case 21 : student->branch ="Bio Tech Engineering";
        break;
    case 22 : student->branch ="Master of Computer Application";//MCA
        break;
    default : student->branch = "";
    }
}

CDatabase::~CDatabase()
{
    if(QSqlDatabase::contains("docmaq"))
        QSqlDatabase::removeDatabase("docmaq");
}
