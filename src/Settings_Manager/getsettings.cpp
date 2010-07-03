#include "getsettings.h"
#include "Settings.h"
#include <QList>
#include <QStringList>
#include <QSettings>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>


GetSettings::GetSettings()
{
    settings=new QSettings(QSettings::SystemScope,"Qogency","DocmaQ");
    createSettingsInterface(0);
}


void GetSettings::setType(const short& type)
{

    switch (type)
    {
    case BONAFIDE:
        cert="Bonafide";
        break;
    case CONDUCT :
        cert="Conduct";
        break;
    case TC      :
        cert="TC";
        break;
    }

}

void GetSettings::getFieldGroups(QStringList &fieldkeys)
{

    settings->beginGroup("certificate/"+cert+"/fields");

    fieldkeys=settings->childGroups();

    settings->endGroup();

}

void GetSettings::getDatabaseDetails(QStringList &databaseDetails)
{
    settings->beginGroup("Database");
    databaseDetails<<settings->value("databasename").toString();
    databaseDetails<<settings->value("hostname").toString();
    databaseDetails<<settings->value("port").toString();
    databaseDetails<<settings->value("username").toString();
    databaseDetails<<settings->value("password").toString();
    settings->endGroup();
}

//for every print this is called
void GetSettings::setCountSno()
{
    QString str;
    settings->beginGroup("certificate");

    settings->value("sessiontotal",str.setNum(settings->value("sessiontotal").toInt()+1));
    settings->value("total",str.setNum(settings->value("total").toInt()+1));
    settings->value(cert+"count/total",str.setNum(settings->value(cert+"count/total").toInt()+1));
    settings->value(cert+"/serialno",str.setNum(settings->value(cert+"/serialno").toInt()+1));
    settings->value(cert+"count/sessiontotal",str.setNum(settings->value(cert+"count/sessiontotal").toInt()+1));
    settings->endGroup();

}

void GetSettings::getSessionInfo(QStringList &info)
{
    //current session info
    info<<settings->value("log/currentsession/username").toString();
    info<<settings->value("log/currentsession/logintime").toString();

    info<<settings->value("certificate/sessiontotal").toString();
    QStringList cert1;
    cert1<<"bonafide"<<"conduct"<<"tc";
    for(int i=0;i<3;i++)
    {
        settings->beginGroup("certificate/"+cert1[i]);
        info<<settings->value("count/sessiontotal",0000).toString();
        info<<settings->value("previousrollno",0000).toString();
        settings->endGroup();
    }
    info<<settings->value("log/previoussession/username").toString();
    info<<settings->value("log/previoussession/logintime").toString();
    info<<settings->value("log/previoussession/logouttime").toString();
}


//this will called for each cert type.type is set in log class(writeCertificateLog) and will be available here as cert
void GetSettings::getSno(QString& sno)
{
    sno=settings->value("certificate/"+cert+"/serialno").toString();
}


void GetSettings::getDateAcademicYear(QList<int>& academicyear)
{

    academicyear<<settings->value("certificate/from").toInt();
    academicyear<<settings->value("certificate/to").toInt();
}


void GetSettings::getPrintPositions(QList<int>& pos,short type)
{

    QStringList fieldkeys;

    setType(type);

    getFieldGroups(fieldkeys);

    short size=fieldkeys.size();

    settings->beginGroup("certificate/"+cert+"/fields");

    for(short i=0;i<size;i++)
    {
        pos<<settings->value(fieldkeys.at(i)+"/x").toInt();
        pos<<settings->value(fieldkeys.at(i)+"/y").toInt();
    }

    settings->endGroup();

}


QString  GetSettings::getDefaultPrinter()
{
    return settings->value("general/defaultprinter").toString();
}

void  GetSettings::createSettingsInterface(int id)
{
    // Settings *st = new Settings(settings,,id);

}

void GetSettings::error1()
{
    if (settings->status()!= QSettings::NoError)
        QMessageBox::critical(0,tr("Settings Manager"),tr("Error occured during fetching the settings.\n\nPlease try again."));
    else  if(QMessageBox::RestoreDefaults==QMessageBox::critical(0,tr("Settings Manager"),tr("\
                                                                                             Error occured during fetching the settings.\n\n\
                                                                                             You can do one of the following\n\n\
                                                                                             1.Press Cancel and Retry  (or)\n\
                                                                                             2.Restore to Initial settings.\n\n\
                                                                                             CAUTION:\n\n\
                                                                                                 Before Restoring to Defaults it is MANDATORY to\n\
                                                                                                 Export the settings to save critical settings\n\
                                                                                                 and Import them after restore."),QMessageBox::RestoreDefaults|QMessageBox::Default,
                                                                 QMessageBox::Cancel,QMessageBox::NoButton))
    {

        QProcess *myProcess = new QProcess(this);
        myProcess->start("./DocmaQ Initializer.exe",NULL);

    }
}

GetSettings::~GetSettings()
{
}
