/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 K.Praneeth <praneethk@in.com>

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

#include "getsettings.h"
#include "Settings.h"
#include <QStringList>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QPrinterInfo>

/*
 * Constructor
 */
GetSettings::GetSettings()
{
    settings=new QSettings(QSettings::SystemScope,"Qogency","DocmaQ");
    cert << "bonafide" << "conduct" << "tc";

    QSettings settings1(QSettings::SystemScope,"DocmaQ","App");
    
    g = settings->childGroups();
    g << settings1.childGroups();

    if(g.size()!=5)
    {
        //groups contains the fields that are mandatory
        QStringList groups,g2;
        groups<<"Certificate"<<"Database"<<"General";
        for(int i=0,j=1;i<3;i++)
        {
            if(!g.contains(groups[i],Qt::CaseInsensitive))
            {
                g2<<"\n"+QString().setNum(j)+". "+groups[i];
                j++;
            }
        }

        if(g2.size() != 0)
            QMessageBox::information(0,"DocmaQ Settings","Some Settings are not Found.\nMissing DocmaQ Settings are being Restored to Defaults... \n\nExisting Settings are Retained in this Restoration.\n\nThe Settings Restored are "+g2.join("")+ ".\n\n You can set them with DocmaQ Settings after Login.");

        if(!g.contains("logins"))
            QMessageBox::information(0,"DocmaQ Settings","Passwords are Reset During the Restoration.\n\n(Admin/Staff) Login possible only through default password.\n\nContact Admin for Assistance. ");

        setupRegistry();
    }
}

/*
 * Setup the Windows Registry
 */
void GetSettings::setupRegistry()
{
    settings->setValue("installdate",QDateTime::currentDateTime().toString());
    settings->setValue("version","2.0");
    settings->setValue("installdir",QDir().absolutePath());

    QSettings *settings1=new QSettings(QSettings::SystemScope,"DocmaQ","App");
    if(!g.contains("logins"))
    {
        //------------setting initial login passwords--------------------------------
        QCryptographicHash hashed(QCryptographicHash::Sha1);
        hashed.addData ("admin", 5 );
        settings1->beginGroup("logins");
        settings1->setValue("admin",hashed.result());
        hashed.reset();

        hashed.addData("snist", 5);
        settings1->setValue("staff",hashed.result());
        settings1->endGroup();
        hashed.reset();
    }

    if(!g.contains("session"))
    {
        settings1->beginGroup("session");

        settings1->setValue("current/username","");
        settings1->setValue("current/login","");

        settings1->setValue("previous/login","No previous logins");
        settings1->setValue("previous/logout","No previous logins");
        settings1->setValue("previous/username","No previous logins");

        settings1->endGroup();
    }
    delete settings1;
    //------------setting database details----------------------------------------
    if(!g.contains("database"))
    {
        settings->beginGroup("database");

        settings->setValue("databasename","sreenidhi");
        settings->setValue("port",3306);
        settings->setValue("hostname","localhost");
        settings->setValue("username","root");
        settings->setValue("password","");

        settings->endGroup();
    }
    //------------setting initial General settings---------------------------------
    if(!g.contains("general"))
    {
        settings->beginGroup("general");

        QString p;
        p = QPrinterInfo::defaultPrinter().printerName();

        if(p.isEmpty())
            p ="";

        settings->setValue("printer",p);
        settings->setValue("mode",false);
        settings->setValue("reminder",2000);
        settings->endGroup();
    }
    //------------setting initial certificate and Position settings-----------------
    if(!g.contains("certificate"))
    {
        settings->beginGroup("certificate");
        settings->setValue("sessiontotal",0000);
        settings->setValue("total",0000);
        settings->setValue("totaltoday",0000);
        settings->setValue("from",2010);
        settings->setValue("to",2011);
        settings->endGroup();

        QList<int> pos[3];

        pos[0] << 76 << 39 << 174 << 39 << 91 << 67 << 168 << 67 << 48 << 77<< 68<< 87<< 103<< 96<< 75<< 105<< 114<< 105<< 19<< 123<< 110<< 123;
        pos[1] << 48 << 25 << 170 << 24 << 60 << 53 << 170 << 53 << 42 << 64 << 72 << 74 << 90 << 84 << 78 << 94;
        pos[2] << 30 << 40 << 156 << 40 << 39 << 65 << 128 << 65 << 173 << 65 << 76 << 75 << 73 << 88 << 71 << 101 << 108 << 114 << 85 << 127 << 82
                << 139 << 98 << 152 << 132 << 165 << 60 << 178 << 105 << 191 << 67 << 204 << 140 << 217 << 86 << 229;

        for(int i=0; i<3 ; i++)
        {
            settings->beginGroup("certificate/"+cert.at(i));

            settings->setValue("count/session",0000);
            settings->setValue("count/total",0000);
            settings->setValue("count/today",0000);
            settings->setValue("serialno",0001);
            settings->setValue("previousrollno","");

            int size1=pos[i].size();

            for(int j=0;j<size1;j++)
            {
                settings->beginGroup("positions");

                settings->setValue(QString().setNum(j),pos[i].at(j));

                settings->endGroup();
            }
            settings->endGroup();
        }
     }
}

/*
 * Get Database Details
 */
void GetSettings::getDatabaseDetails(QStringList &databaseDetails)
{
    databaseDetails.clear();
    settings->beginGroup("Database");
    databaseDetails<<settings->value("databasename","").toString();
    databaseDetails<<settings->value("hostname","").toString();
    databaseDetails<<settings->value("port","").toString();
    databaseDetails<<settings->value("username","").toString();
    databaseDetails<<settings->value("password","").toString();
    settings->endGroup();
}

/*
 * Get Session Information
 */
void GetSettings::getSessionInfo(QStringList &info)
{
    QSettings *settings1=new QSettings(QSettings::SystemScope,"DocmaQ","App");
    info.clear();
    //current session info
    info<<settings1->value("session/current/username","").toString();
    info<<settings1->value("session/current/login","").toString();

    for(int i=0;i<3;i++)
    {
        settings->beginGroup("certificate/"+cert[i]);
        info<<settings->value("count/session","").toString();
        info<<settings->value("previousrollno","").toString();
        settings->endGroup();
    }
    info<<settings1->value("session/previous/username","").toString();
    info<<settings1->value("session/previous/login","").toString();
    info<<settings1->value("session/previous/logout","").toString();
    delete settings1;
}

/*
 * Get Serial No.
 */
void GetSettings::getSno(QList<int>& sno)
{
    sno.clear();

    for(int i =0;i<3;i++)
        sno << settings->value("certificate/"+cert[i]+"/serialno",0).toInt();
}

/*
 * Get Date,Academic Year
 */
void GetSettings::getDateAcademicYear(QStringList& info)
{
    info.clear();

    info << settings->value("certificate/date","").toDate().toString("dd.MMM.yyyy");
    info << settings->value("certificate/from","").toString();
    info << settings->value("certificate/to","").toString();
}

/*
 * Sets Print Positions
 */
void GetSettings::getPrintPositions(QList<int>& pos,int type)
{
    pos.clear();

    QStringList fieldkeys;

    settings->beginGroup("certificate/"+cert[type]+"/positions");

    fieldkeys=settings->childKeys();

    int size=fieldkeys.size();
    QString temp;

    for(int i=0;i<size;i++)
        pos.append(settings->value(temp.setNum(i),0).toInt());

    settings->endGroup();
}

/*
 * Get Mode
 */
void GetSettings::getMode( bool & mode)
{
    mode=settings->value("general/mode",true).toBool();
}

/*
 * Set Mode
 */
void GetSettings::setMode(bool &mode)
{
    settings->setValue("general/mode",mode);
}

/*
 * Get Printer Details
 */
void GetSettings::getPrinter( QString& printer)
{
    printer= settings->value("general/printer","").toString();
}

/*
 * Setup Settings Interface
 */
void  GetSettings::createSettingsInterface(QWidget *parent,int user)
{
    st = new Settings(user,settings,this,parent);
    if(parent)
 	st->move(parent->width() /2 - 235 , parent->height() / 2 - 241);
    st->show();
}

/*
 * Destructor
 */
GetSettings::~GetSettings()
{
}
