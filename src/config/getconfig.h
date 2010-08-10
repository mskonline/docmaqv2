/* DocmaQ Configure, Configuration Interface for DocmaQ v2.0
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

#ifndef GetConfig_H
#define GetConfig_H

#include <QList>
#include <QStringList>
#include <QSettings>
#include <QDir>
#include <QWidget>

enum certificateType{BONAFIDE,CONDUCT,TC};

class GetConfig:public QObject
{

    Q_OBJECT
    
public :
QStringList g;
GetConfig();
~GetConfig();

void setType( short& type);

//-----database
void getDatabaseDetails(QStringList& databasedetails);

public slots:

void createSettingsInterface();

private:

QSettings *settings;
QStringList databaseDetails;

void setupRegistry();
};
#endif // GetConfig_H
