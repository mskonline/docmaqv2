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

#ifndef LOGS_H
#define LOGS_H

#include <QFile>
#include <QTextStream>
#include<QCloseEvent>
#include <QStringList>
#include <QSettings>

class Logmanager;

class d_log:public QObject
{
    Q_OBJECT

public :
    Logmanager *lm;
    d_log();
    ~d_log();

    void writeCertificateLog(const QString& rollno,const int& type);
    void startSession(const QString& userid,const QString& username);
    void endSession();

public slots:
    void createlogmanager(QWidget *);
    void closelog();
    void openlog();

private :
    QStringList cert;
    enum certificateType{BONAFIDE,CONDUCT,TC};
    QSettings *settings;
    QFile file;
    QTextStream out;
    QString dt,path;//path=current path
    QString username;
    QFile sessionFile;

    void closeEvent(QCloseEvent *e);

signals:
    void backupSignal(int);
};

#endif
