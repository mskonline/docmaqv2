/*  DocmaQ Configure, Configuration Interface for DocmaQ v2.1
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

#include "getconfig.h"
#include "config.h"
#include <QSettings>
#include <QMessageBox>
#include <QDate>
#include <QCryptographicHash>
#include <QPrinterInfo>

/*
 * Constructor
 */
GetConfig::GetConfig()
{
    settings=new QSettings(QSettings::SystemScope,"Qogency","DocmaQ");

    //collecting existing fields in g1
    QSettings settings1(QSettings::SystemScope,"DocmaQ","App");
    
    g = settings->childGroups();
    g << settings1.childGroups();

    if(g.size()!=5)
       setupRegistry();

    createSettingsInterface();
}

/*
 * Setup the Windows Registry
 */
void GetConfig::setupRegistry()
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
    //--------------------setting initial General settings---------------------------------
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
    //----------------setting initial certificate and Position settings-----------------
    if(!g.contains("certificate"))
    {
        settings->beginGroup("certificate");
        settings->setValue("sessiontotal",0000);
        settings->setValue("total",0000);
        settings->setValue("totaltoday",0000);
        settings->setValue("from",2010);
        settings->setValue("to",2011);
        settings->endGroup();

        QStringList cert;
        QList<int> pos[3];
        cert<<"bonafide"<<"conduct"<<"tc";

        pos[0] << 76 << 39 << 174 << 39 << 91 << 67 << 168 << 67 << 48 << 77<< 68<< 87<< 103<< 96<< 75<< 105<< 114<< 105<< 19<< 123<< 110<< 123;
        pos[1] << 48 << 25 << 170 << 24 << 60 << 53 << 170 << 53 << 42 << 64 << 72 << 74 << 90 << 84 << 78 << 94;
        pos[2] << 30 << 40 << 156 << 40 << 39 << 65 << 128 << 65 << 173 << 65 << 76 << 75 << 73 << 88 << 71 << 101 << 108 << 114 << 85 << 127 << 82
                << 139 << 98 << 152 << 132 << 165 << 60 << 178 << 105 << 191 << 67 << 204 << 140 << 217 << 86 << 229;

        int size=cert.size();
        for(int i=0;i<size;i++)
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
void GetConfig::getDatabaseDetails(QStringList &databaseDetails)
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
 * Setup Config Interface
 */
void  GetConfig::createSettingsInterface()
{
    Config *st = new Config(settings,this);
}

/*
 * Destructor
 */
GetConfig::~GetConfig()
{}
