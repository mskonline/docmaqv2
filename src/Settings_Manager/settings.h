#ifndef SETTINGS_H
#define SETTINGS_H

#include "ui_settings.h"
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QSharedPointer>

class GetSettings;

class Settings : public QWidget,public Ui::settings
{
    Q_OBJECT

public:

    Settings(QSettings *settings,GetSettings *getSettings,QWidget *parent = 0);
    void closeEvent(QCloseEvent * e);
    ~Settings();

private slots:
    void selectFunction(int);

    //Database page
    void adminAuthentication();

    void saveDatabaseSettings(const QString &name,const QString &text)
    {

        settings->setValue("Database/"+name,text);
    }


    void databaseSettingsChanged( const QString &text)
    {
        QString name= sender()->objectName();
        name.chop(2);
        saveDatabaseSettings(name,text);
    }

    void undoDatabaseSettings();
    void enableUndoReconnectButtons();

    //Account page
    void on_newPasswordLE_editingFinished();
    void on_changeButton_clicked();

    //Certificate page
    void prepareCertificateSettings();
    void saveCertificateSettings();
    void saveOrder();

    //Print page
    void preparePrintSettings();
    void setXY(QString);
    void savePrintPositions();
    void savePrinter(const QString &printer);

    //Log page
    void on_browseButton_clicked();
    void on_defaultButton_clicked();
    void saveLogDuration();

    //General Page

    void saveFullscreen(bool);
    void saveTheme(int index);
    void on_importButton_clicked();
    void on_exportButton_clicked();
    void saveAcademicYear();




private:
    enum certificatetype{BONAFIDE,CONDUCT,TC};
    QSettings *settings;
    QString cert1;
    QFile file;
    QTextStream out;
    bool flag[6],f[7];
    GetSettings *getSettings;
    QStringList databaseDetails,stringType;


    void prepareDatabaseSettings();
    void prepareAccountSettings();

    void prepareLogSettings();
    void fillLogSettings();
    void prepareGeneralSettings();
    void getPrinters();
    void generateRegFile(QString key,QString path);
    void setOrder();

    void decideCert1( QString &cert);
    void decideCert2( short &type);

    inline void messageBox(char *,char *);
    inline void criticalMessageBox(char *,char *);
    void error();

signals:

    void changeTheme(QString theme);
    void updatePrintPositions(int);
    void updateOrder(int);
    void updatePrinter(int);
    void updateCertificateSettings(int);
    void updateAcademicYear(int);
    void checkLogDirectory(int);
    void removeExcessLogFiles(int);


};


#endif // SETTINGS_H
