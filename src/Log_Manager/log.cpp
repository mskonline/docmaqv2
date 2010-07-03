#include <QDir>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QProgressDialog>
#include "log.h"
#include "../settings/getsettings.h"
#include "../log_manager/logmanager.h"


log::log()
{
    getSettings=new GetSettings();

    path="./logs/certificate/";
    
    lm=NULL; //initially there is no logmanager widget (mandatory)
    
    settings= new QSettings(QSettings::SystemScope,"Qogency","DocmaQ");
    
    //setting session counter to 0 for the same day to display no of prints in a day

    QStringList cert;
    cert<<"bonafide"<<"conduct"<<"tc";


    if (settings->value("general/date").toDate()==QDate::currentDate())
    {
        settings->beginGroup("certificate");

        settings->setValue("sessiontotal",0);
        for(int i=0;i<3;i++)
        {
            settings->setValue(cert.at(i)+"count/sessiontotal",0);
        }
        settings->endGroup();
    }

    else
    {
        uint r=settings->value("log/reminder").toInt();
        settings->setValue("log/reminder",--r);
        
    }


    if((settings->value("log/reminder").toInt()==0))
    {

        settings->setValue("log/reminder",1000);
        if(1==QMessageBox::question(0,"Log Manager","Log files have accumulated.Please take a Back-Up to prevent their future loss due to System Migration or Formatting of your OS\
                                    Go to Settings->Log Page.\n\nKindly Contact Admin for Assistance",tr(" &Back Up now "), tr(" &Not now "),QString(), 1, 0));
        {
            connect(this,SIGNAL(backupSignal(int)),getSettings,SLOT(createSettingsInterface()));
            emit backupSignal(3);
        }
    }
    
    //------------------creating a temp certificate log files(certificate/session) for this session locally

    if(!QFile::exists(path))
        QDir().mkdir(path);

    dt = QDate::currentDate().toString("dd.MMM.yyyy");
    file.setFileName(path+ dt + ".Log");
    out1.setDevice(&file);
    file.open(QIODevice::Append|QIODevice::Text);

    sessionFile.setFileName("./logs/session/"+dt+".Log");

    startSession("NT012","Manohar");
    endSession();
    startSession("NT012","earar");endSession();
    startSession("NT012","ianuar");endSession();
    startSession("NT012","ranohar");
    endSession();


    writeCertificateLog("06311A05A4",0);


}


void log::writeCertificateLog(const QString& rollno,const short& type)
{
    out1.setDevice(&file);
    getSettings->setType(type);//type is set only here for everyone to access
    getSettings->setCountSno();
    out1<<rollno<<" "<<getSettings->cert<<" "<<QDate::fromString(dt,"dd.MMM.yyyy").toString("dd-MMM-yyyy")<<" "<<QTime::currentTime().toString("hh:mm:ss-A")<<" "<<username<<"\r\n";

}

void log::startSession(const QString& userid,const QString& username)
{

    settings->setValue("log/currentsession/username",username);
    settings->setValue("log/currentsession/logintime",QTime::currentTime().toString("hh:mm:ss-A"));
    sessionFile.open(QIODevice::Append|QIODevice::Text);
    out.setDevice(&sessionFile);

    QStringList sessionRecord;
    sessionRecord<<userid<<username<<QDate::fromString(dt,"dd.MMM.yyyy").toString("dd-MMM-yyyy")<<QTime::currentTime().toString("hh:mm:ss-A");

   int size=sessionRecord.size();
    for(int i=0;i<size;i++)
    {
        if(i!=size-1)
            out << sessionRecord[i]+" ";
        else
            out << sessionRecord[i]+"\r\n";
    }
    sessionFile.close();

}

void log::endSession()
{
    settings->setValue("log/previoussession/username",username);
    settings->setValue("log/previoussession/logintime",settings->value("log/currentsession/logintime"));
    settings->setValue("log/previoussession/logouttime",QTime::currentTime().toString("hh:mm:ss-A"));

    sessionFile.open(QIODevice::Append|QIODevice::Text);
    out.setDevice(&sessionFile);

    QString last;

    out.seek(out.pos()-2);//to go back to append before \r\n

    out<<" "<<QTime::currentTime().toString("hh:mm:ss-A")<<" "<<settings->value("certificate/sessiontotal").toString()+"\r\n";

    sessionFile.close();

}


void log::createlogmanager()
{
    file.flush();
    if (!lm)
    {
        lm = new Logmanager();
        lm->show();
    }
    else if (lm->isHidden()||lm->isMinimized())
    {
        lm->showNormal();
    }
    else
        lm->activateWindow();
}
void log::closeEvent(QCloseEvent *e)
{
    qDebug()<<"close";
    file.close();
}

log::~log()
{

}
