/* DocmaQ v2.0, Credential Publishing System
    Copyright (C) 2010 K.Praneeth <praneethk@in.com>

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

#include "log.h"
#include "../log_manager/logmanager.h"
#include <QDir>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QSettings>
#include <QProgressDialog>

d_log::d_log()
{
    path="./logs/certificate/";
    
    lm=NULL; //initially there is no logmanager widget (mandatory)
    
    settings= new QSettings(QSettings::SystemScope,"Qogency","DocmaQ");
    //setting session counter to 0 for the same day to display no of prints in a day

    cert<<"Bonafide"<<"Conduct"<<"TC";

    settings->beginGroup("certificate");
    for(int i=0;i<3;i++)
        settings->setValue(cert.at(i)+"/count/session",0);
    settings->endGroup();

    QDate c = QDate::currentDate();

    if(settings->value("general/date") != c)
    {
        //resetting today's count
        settings->beginGroup("certificate");

        settings->setValue("totaltoday",0);

        for(int i=0;i<3;i++)
            settings->setValue(cert.at(i)+"/count/today",0);

        settings->endGroup();

        settings->setValue("general/date",c);

        uint r=settings->value("general/reminder").toInt();
        settings->setValue("general/reminder",--r);
    }

    if((settings->value("general/reminder").toInt()==0))
    {

        settings->setValue("general/reminder",2000);
        QMessageBox::information(0,"DocmaQ Log Manager","Log files have accumulated.Please take a Back-Up to prevent their loss due to System Migration or Formatting of your OS\
                                    Go to Settings->Log Page.\n\nKindly Contact Admin for Assistance",tr(" &Back Up now "), tr(" &Later "),QString(), 1, 0);
    }

    //creating a temp certificate log files(certificate/session) for this session locally
    if(!QFile::exists(path))
        QDir().mkdir(path);

    if(!QFile::exists("./logs/session/"))
        QDir().mkdir("./logs/session/");

    dt = c.toString("dd.MMM.yyyy");
    file.setFileName(path+ dt + ".Log");
    out.setDevice(&file);
    file.open(QIODevice::Append|QIODevice::Text);

    sessionFile.setFileName("./logs/session/"+dt+".Log");
}

void d_log::startSession(const QString& userid,const QString& username)
{
    settings->beginGroup("certificate");
    settings->setValue("date",QDate::currentDate());
    settings->setValue("sessiontotal",0);

    for(int i = 0; i < 3; ++i)
        settings->setValue(cert[i] + "/previousrollno","");

    settings->endGroup();

    this->username = username;
    QSettings *settings1=new QSettings(QSettings::SystemScope,"DocmaQ","App");
    settings1->setValue("session/current/username",username);
    settings1->setValue("session/current/login",QTime::currentTime().toString("hh:mm:ss A"));
    delete settings1;

    sessionFile.open(QIODevice::Append|QIODevice::Text);
    out.setDevice(&sessionFile);

    out<<userid<<"~"<<username<<"~"<< dt <<"~"<<QTime::currentTime().toString("hh:mm:ss A") <<
            "~" << "" << "~"  << "" <<  "\r\n";

    sessionFile.close();
    out.setDevice(&file);
}

void d_log::writeCertificateLog(const QString& rollno,const int &c)
{
    QString str;

    settings->beginGroup("certificate");
    out << rollno << "~" <<  settings->value(cert[c] + "/serialno","0").toString()<<"~"<<cert[c]<<"~"<<settings->value("date","").toDate().toString("dd.MMM.yyyy")<<
            "~"<<QTime::currentTime().toString("hh:mm:ss A")<<"~"<<username<<"\r\n";

    switch (c)
    {
    case BONAFIDE:
    case CONDUCT:
        settings->setValue(cert[c] + "/serialno",str.setNum(settings->value(cert[c]+"/serialno",0).toInt()%9999 +1));
        break;
    case TC:
        settings->setValue(cert[c] + "/serialno",str.setNum(settings->value(cert[c]+"/serialno",0).toInt()%99999+1));
     }

    settings->setValue("sessiontotal",str.setNum(settings->value("sessiontotal",0).toInt()+1));
    settings->setValue("total",str.setNum(settings->value("total",0).toInt()+1));
    settings->setValue("totaltoday",str.setNum(settings->value("totaltoday",0).toInt()+1));
    settings->setValue(cert[c] + "/previousrollno",rollno);
    settings->setValue(cert[c] + "/count/total",str.setNum(settings->value(cert[c]+"/count/total",0).toInt()+1));
    settings->setValue(cert[c] + "/count/today",str.setNum(settings->value(cert[c] + "/count/today",0).toInt() + 1));
    settings->setValue(cert[c] + "/count/session",str.setNum(settings->value(cert[c]+"/count/session",0).toInt()+1));
    settings->endGroup();

    file.flush();
}

void d_log::closelog()
{
    file.close();
}

void d_log::openlog()
{
    file.setFileName(path+ dt + ".Log");
    file.open(QIODevice::Append|QIODevice::Text);
    out.setDevice(&file);
}

void d_log::endSession()
{
    QString c;
    QSettings *settings1=new QSettings(QSettings::SystemScope,"DocmaQ","App");
    settings1->setValue("session/previous/username",username);

    c = dt + " " + settings1->value("session/current/login","").toString();
    settings1->setValue("session/previous/login",c);

    c = dt + " " + QTime::currentTime().toString("hh:mm:ss A");
    settings1->setValue("session/previous/logout",c);

    sessionFile.open(QIODevice::Append|QIODevice::Text);
    out.setDevice(&sessionFile);

    //go back to append
    out.seek(out.pos() - 5);
    out << "~" << QTime::currentTime().toString("hh:mm:ss A")<<"~"<<settings->value("certificate/sessiontotal").toString()+"\r\n";

    sessionFile.close();
    file.close();

    if(file.size() == 0)
        QDir().remove(file.fileName());

    delete settings1;
}

void d_log::createlogmanager(QWidget *parent)
{
    file.flush();
    closelog();

    lm = new Logmanager(parent);
    lm->move(parent->width()/2 - 338, parent->height() /2 - 275);
    lm->show();

    connect(lm,SIGNAL(lmclose()),this,SLOT(openlog()));
}

void d_log::closeEvent(QCloseEvent *e)
{
    file.close();
}

d_log::~d_log()
{
}
