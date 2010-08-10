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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "ui_settings.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QWidget>

class GetSettings;

class Settings : public QWidget, public Ui::settings
{
    Q_OBJECT

public:
    Settings(int,QSettings *,GetSettings *,QWidget *parent = 0);
    void closeEvent(QCloseEvent * e);
    ~Settings();

private slots:
    void selectFunction(int id);

    //Database page
    void adminAuthentication();
    void databaseSettingsChanged();
    void checkConnectivity();
    void fillDatabaseSettings();
    void enableReconnectButton();

    //Account page
    void on_newPasswordLE_textEdited();
    void on_changeButton_clicked();

    //Certificate page
    void decideCert2();
    void prepareCertificateSettings();
    void setDateAcademicYearFlag();
    void setModeFlag();
    void on_serialSB_editingFinished();

    //Print page
    void decideCert1();
    void insertFields();
    void preparePrintSettings();
    void setXY(int index);
    void setPrintPositionsFlag();
    void setPrinterFlag( const QString &printer);

    //Log page
    void on_browseButton_clicked();
    void on_backupButton_clicked();

    //General Page
    void on_importButton_clicked();
    void on_exportButton_clicked();

    //on ok button pressed
    void saveSettings();

private:
    QFile file;
    QList<int>pos[3],x[3],y[3],ser;
    QTextStream out;
    bool flag[6],f[8],au;
    GetSettings *getSettings;
    QStringList databaseDetails,fields[3];
    int type1,type2;//cert1 for print page,cert2 for certificate page
    QString cert1,cert2,fileName;//similarly type1 these hold current type being dealt in respective pages
    QFile serverfile;
    QString directory;
    QSettings *settings;
    QList <int> siglist;
    enum certificateType{BONAFIDE,CONDUCT,TC};

    void prepareDatabaseSettings();
    void prepareAccountSettings();
    void prepareLogSettings();
    void fillLogSettings();
    bool createServerFile( QString& filename, QString &path, QString &dir);
    void prepareGeneralSettings();
    void savep(QString& cert,int &type);

signals:
    void sendSignal(QList <int> &);
};
#endif // SETTINGS_H
