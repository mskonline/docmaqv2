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

#ifndef GetSettings_H
#define GetSettings_H

#include <QList>
#include <QStringList>
#include <QSettings>
#include <QWidget>

class Settings;

class GetSettings:public QObject
{
    Q_OBJECT

public :
Settings *st;
 
GetSettings();
~GetSettings();

QStringList cert;
QStringList g;
//-----database
void getDatabaseDetails(QStringList& databasedetails);

//-----certificate
void getSessionInfo(QStringList& info);
void setCountSno();
void getSno( QList<int>& sno);
//-----general
void getDateAcademicYear(QStringList& info);
void getPrintPositions(QList<int>& pos,int type);
void getMode( bool & mode);
void setMode(bool &mode);
void getPrinter( QString & printer);

public slots:
void createSettingsInterface(QWidget *,int);

private:
QSettings *settings;
QStringList databaseDetails;

void setupRegistry();
};
#endif // GetSettings_H
