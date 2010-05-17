#include "log.h"
#include "../Log_Manager/logmanager.h"
#include <QDir>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QProgressDialog>
#include <QApplication>

logs::logs(QTextEdit *lg)
{
    lgs=lg;
    
    path="./logs/";//if there is no LAN connection
    
    lm=NULL; //initially there is no logmanager widget (mandatory)
    
    
    settings= new QSettings( QSettings::SystemScope,"Qogency","DocmaQ");
    
    //setting counter to 0 for a new day to display no of prints in a day
    if (!(settings->value("certificatecount/date").toDate()==QDate::currentDate()))
    {
        settings->beginGroup("certificatecount");
        settings->setValue("date",QDate::currentDate());
        settings->setValue("bonafide/bonafidecount2day",0);
        settings->setValue("tc/tccount2day",0);
        settings->setValue("bonafideconduct/bonafideconductcount2day",0);
        settings->endGroup();
        uint r=settings->value("generalsettings/reminder").toInt();
        settings->setValue("generalsettings/reminder",--r);
        
    }
    
    //----------------- Check for any temporary log  files and copy to log dir in server
    if (settings->value("generalsettings/logmode","local").toString()=="central")
    {
        
        path=settings->value("generalsettings/logpath").toString();
        if (settings->status()!= QSettings::NoError)
        {
            QMessageBox::information(this,"Settings Manager","     Error Occured During Fetching The Settings\n \
                                     You can fetch Settings Manually by visiting \
                                     HKEY_LOCAL_MACHINE\\Software\\Quogency\\DocmaQ \
                                     start->run->regedit    ");
        }
        
        if (!path.endsWith("\\"))
            path=path+"\\";

        if(!QFile::exists(path))
            QDir().mkdir(path);
        
    }
    
    if(!QFile::exists("logs"))
        QDir().mkdir("logs");

    if (settings->value("generalsettings/reminder").toInt()==0)
    {
        settings->setValue("generalsettings/reminder",30);
        removeExcessLogFiles();
    }
    
    //------------------creating a temp log file for this session locally
    
    dt = QDate::currentDate().toString("dd.MMM.yyyy");
    file.setFileName("./logs/" + dt + ".Log");
    out.setDevice(&file);
    file.open(QIODevice::Append);
}

void logs::checkLogDirectory()
{
    QDir dir("./logs/");

    QStringList files;
    files = dir.entryList(QStringList("*.Log"),QDir::Files,QDir::Time);

    uint count=files.count();

    for (uint i=1;i<count;i++)
    {
        if (createServerFile(files[i])==false)
                break;
    }
    files.clear();//emptying stringlist
}



void logs::removeExcessLogFiles()
{
    QDir logDir(path);

    uint duration =settings->value("generalsettings/logduration").toInt();

    QStringList files = logDir.entryList(QStringList("*.Logc"),QDir::Files,QDir::Time|QDir::Reversed);
    uint size=files.count();

    if (size>duration)
    {
        if ( QMessageBox::question(this, tr("Log Manager"),
                                   tr("Log files have reached their limit.\nPlease take a \"backup\" of Log files before freeing up memory.\nContact Software Dept for assistance... "),
                                   tr("&Delete now"), tr("&Not now"), QString(), 1, 0));
        {
            QProgressDialog progressDialog(this);
            progressDialog.setCancelButtonText(tr("&Cancel"));
            progressDialog.setRange(0, size-duration);
            progressDialog.setWindowTitle(tr("Removing Log Files...."));

            QFile logfile;//holds each file to be removed

            for (uint i=0;i<size-duration;i++)
            {
                progressDialog.setValue(i);
                progressDialog.setLabelText(tr("Removing file number %1 of %2...")
                                            .arg(i).arg(size-duration));
                qApp->processEvents();

                if (progressDialog.wasCanceled())
                    break;
                logfile.setFileName(path+files[i]);

                if (!logfile.remove())
                {
                    QMessageBox::warning(this,tr("Log Manager"),tr("unable to remove log files....!!"));
                    break;
                }
            }
        }

    }

}


void logs::write(const QString& str)
{
    lgs->append(str);
    out << str;
    
}

void logs::save()
{

    /*
     *if central repository is maintained then to view todays log or to
     *update log b/w local and central file.
     */
    
    if (settings->value("generalsettings/logmode")=="central")
    {
        
        //to  flush & Close the file and reopen in readonly mode in createserverfile()
        checkLogDirectory();//to copy  files to server only if there is no such log exists
    }
    
    else
        file.flush();//just flush the buffer in order to view todays log locally
    
}

bool logs::createServerFile(const QString& filename)
{
    bool ok;
    QString localfile("./logs/"+filename);

    ok=QFile::copy( localfile,path+filename+"c");//copying to central repository
    if (ok)
    {
        QFile::rename( localfile,localfile+"c");

        return true;
    }


    if (ok&&QFile().error()==QFile::CopyError||QFile::TimeOutError||QFile::UnspecifiedError)
    {
        QMessageBox::information(this,tr("File Error"),tr("Cannot Create Log in server,\nContact Software Dept(Admin)"),QMessageBox::Ok);
        return false;
    }
    
}

void logs::createlogmanager()
{
    file.flush();
    if (!lm)
    {
        lm = new Logmanager();
        lm->show();
    }
    else if (lm->isHidden()||lm->isMinimized())
    {
        lm->loadTextFile(QDate::currentDate());
        lm->showNormal();
    }
    else
        lm->activateWindow();
}

void logs::setCount(int count2day,int totalcount,int type)
        //called at close of app
{
    QString temp,str1,str2,str3;
    QStringList count;
    switch (type)
    {
    case BONAFIDE:
        str1="bonafide";
        str2="bonafidecount2day";
        str3="totalbonafidecount";
        break;
    case CONDUCT :
        str1="bonafideconduct";
        str2="bonafideconductcount2day";
        str3="totalbonafideconductcount";
        break;
    case TC      :
        str1="tc";
        str2="tccount2day";
        str3="totaltccount";
        break;
    }
    settings->beginGroup("certificatecount/"+str1);
    
    
    settings->setValue(str2,count2day);
    
    settings->setValue(str3,totalcount);
    
    settings-> endGroup();
    
}


QList <int> logs::getCount(int type)
{
    QString temp,str1,str2,str3;
    QList <int> count;
    
    switch (type)
    {
    case BONAFIDE:
        str1="bonafide";
        str2="bonafidecount2day";
        str3="totalbonafidecount";
        break;
    case CONDUCT :
        str1="bonafideconduct";
        str2="bonafideconductcount2day";
        str3="totalbonafideconductcount";
        break;
    case TC      :
        str1="tc";
        str2="tccount2day";
        str3="totaltccount";
        break;
    }
    
    settings->beginGroup("certificatecount/"+str1);
    
    count<<settings->value(str2,0000).toInt();
    count<<settings->value(str3,0000).toInt();
    
    settings-> endGroup();
    return count;
    
}



QStringList logs::getLastSnoRollNo(int type)
{
    QString str1,str2,str3;
    snorollno.clear();
    switch (type)
    {
    case BONAFIDE:
        str1="bonafide";
        str2="bonafidecount2day";
        str3="totalbonafidecount";
        break;
    case CONDUCT :
        str1="bonafideconduct";
        str2="bonafideconductcount2day";
        str3="totalbonafideconductcount";
        break;
    case TC      :
        str1="tc";
        str2="tccount2day";
        str3="totaltccount";
        break;
    }

    settings->beginGroup("certificatecount/"+str1);
    snorollno<< settings->value("lastrollno").toString();
    snorollno<< settings->value("serialno").toString();
    settings->endGroup();

    return snorollno;
}

//for every print this is called
void logs::setLastSnoRollNo(int serialno ,QString rollno,int type)
{
    QString str1,str2,str3;
    snorollno.clear();
    switch (type)
    {
    case BONAFIDE:
        str1="bonafide";
        str2="bonafidecount2day";
        str3="totalbonafidecount";
        break;
    case CONDUCT :
        str1="bonafideconduct";
        str2="bonafideconductcount2day";
        str3="totalbonafideconductcount";
        break;
    case TC      :
        str1="tc";
        str2="tccount2day";
        str3="totaltccount";
        break;
    }
    
    settings->beginGroup("certificatecount/"+str1);
    
    settings->setValue("lastrollno",rollno);
    settings->setValue("serialno",serialno);
    
    settings-> endGroup();
}
//21.01.2010 11:56:55
void logs::setCurrentYear(int from , int to)
{
	settings->beginGroup("generalsettings/currentyear");
	settings->setValue("from",from);
	settings->setValue("to",to);
	settings-> endGroup();

}
QStringList logs::getCurrentYear()
{
	QStringList year;
	settings->beginGroup("generalsettings/currentyear");
	year<<settings->value("from").toString();
	year<<settings->value("to").toString();
	settings-> endGroup();
	return year;

}
