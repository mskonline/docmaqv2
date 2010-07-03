#ifndef SETTINGS_H
#define SETTINGS_H

#include "ui_settings.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>


class GetSettings;

class Settings : public QWidget,public Ui::settings
{
    Q_OBJECT

public:

    Settings(QSettings *settings,GetSettings *getSettings,int id=0,QWidget *parent = 0);
    void closeEvent(QCloseEvent * e);
    ~Settings();

private slots:
    void selectFunction(int id);

    //Database page
    void adminAuthentication();

    void databaseSettingsChanged( const QString &text)
    {

        QString name= sender()->objectName();
        name.chop(2);
        saveDatabaseSettings(name,text);
    }

    void saveDatabaseSettings(const QString &name,const QString &text)
    {
        f[1]=true;
        //settings->setValue("Database/"+name,text);
        // settings->sync();
    }

    void undoDatabaseSettings();
    void enableUndoReconnectButtons();

    //Account page
    void on_newPasswordLE_textEdited();
    void on_changeButton_clicked();

    //Certificate page
    void prepareCertificateSettings();
    void saveCertificateSettings();
    void saveMode();

    //Print page
    void decideCert1();
    void decideCert2();

    void insertFields();
    void preparePrintSettings();
    void setXY(int index);
    void savePrintPositions();
    void savePrinter(const QString &printer);

    //Log page
    void on_browseButton_clicked();
    void on_backupButton_clicked();
    void saveLogPath();

    //General Page

    void on_importButton_clicked();
    void on_exportButton_clicked();
    void saveAcademicYear();


private:


    QFile file;
    QTextStream out;
    bool flag[6],f[7];
    GetSettings *getSettings;
    QStringList databaseDetails,stringType,fields[3];
    short type1;//cert1 for print page,cert2 for certificate page
    QString cert1,cert2,fileName;//similarly type1 these hold current type being dealt in respective pages
    QFile serverfile;
    QString directory;
    QSettings *settings;


    void prepareDatabaseSettings();
    void prepareAccountSettings();

    void prepareLogSettings();
    void fillLogSettings();
    bool createServerFile(const QString& filename,const QString &path,const QString &dir);
    void prepareGeneralSettings();

    void generateRegFile(QString key,QString path);

    void fillSerial();

signals:

    void sendSignal(int id);

};


#endif // SETTINGS_H
