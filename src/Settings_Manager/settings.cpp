#include "settings.h"
#include "getsettings.h"

#include <QDebug>
#include <QProgressDialog>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QProcess>
#include <QPrinterInfo>
#include <QCloseEvent>

Settings::Settings(QSettings *settings,GetSettings* getSettings,int id,QWidget *parent)
    : QWidget(parent,Qt::WindowCloseButtonHint)
{

    //gets limited settings for interface fill up
    this->getSettings=getSettings;
    
    this->settings = settings;
    
    setupUi(this);
    show();
    connect(adminPasswordLE,SIGNAL(returnPressed()),this,SLOT(adminAuthentication()));

    //connecting signals
    connect(stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(selectFunction(int)));
    
    //print page
    connect(bonafideRB1,SIGNAL(clicked()),this,SLOT(decideCert1()));
    connect(conductRB1,SIGNAL(clicked()),this,SLOT(decideCert1()));
    connect(tcRB1,SIGNAL(clicked()),this,SLOT(decideCert1()));
    
    connect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setXY(int)));
    connect(xSB,SIGNAL(editingFinished()),this,SLOT(savePrintPositions()));
    connect(ySB,SIGNAL(editingFinished()),this,SLOT(savePrintPositions()));
    
    //database page
    
    connect(databasenameLE,SIGNAL(textChanged(const QString &)),this,SLOT(databaseSettingsChanged(const QString &)));
    connect(hostnameLE,SIGNAL(textChanged(const QString &)),this,SLOT(databaseSettingsChanged(const QString &)));
    connect(portLE,SIGNAL(textChanged(const QString &)),this,SLOT(databaseSettingsChanged(const QString &)));
    connect(usernameLE,SIGNAL(textChanged(const QString &)),this,SLOT(databaseSettingsChanged(const QString &)));
    connect(passwordLE,SIGNAL(textChanged(const QString &)),this,SLOT(databaseSettingsChanged(const QString &)));
    
    connect(databasenameLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableUndoReconnectButtons()));
    connect(portLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableUndoReconnectButtons()));
    connect(hostnameLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableUndoReconnectButtons()));
    connect(usernameLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableUndoReconnectButtons()));
    connect(passwordLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableUndoReconnectButtons()));
    
    connect(undoButton,SIGNAL(clicked()),this,SLOT(undoDatabaseSettings()));
    //connect(reconnectButton,SIGNAL(clicked()),appmanager,SLOT(connect(databaseDetails)));
    
    //Log settings

    
    //Certificate page

    connect(bonafideRB2,SIGNAL(clicked()),this,SLOT(decideCert2()));
    connect(conductRB2,SIGNAL(clicked()),this,SLOT(decideCert2()));

    connect(serialSB,SIGNAL(editingFinished()),this,SLOT(saveCertificateSettings()));
    connect(dateEdit,SIGNAL(editingFinished()),this,SLOT(saveCertificateSettings()));
    connect(fromSB,SIGNAL(editingFinished()),this,SLOT(saveAcademicYear()));
    connect(toSB,SIGNAL(editingFinished()),this,SLOT(saveAcademicYear()));
    
    //General Settings
    
    connect(printersCombo,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(savePrinter(const QString)));

    connect(databaseRB,SIGNAL(clicked()),this,SLOT(saveMode()));
    connect(manualRB,SIGNAL(clicked()),this,SLOT(saveMode()));

    int laststackwidget=settings->value("general/laststackwidget",0).toInt();
    listWidget->setCurrentRow(laststackwidget,QItemSelectionModel::ToggleCurrent);

    selectFunction(laststackwidget);

}

void Settings::selectFunction(int id)
{

    //flags ensure each functions to be called once
    switch(id)
    {
    case 0:if(!flag[0])preparePrintSettings();break;
    case 1:if(!flag[1])prepareDatabaseSettings();break;
    case 2:if(!flag[2])prepareAccountSettings();break;
    case 3:if(!flag[3])prepareLogSettings();break;
    case 4:if(!flag[4])prepareCertificateSettings();break;
    case 5:if(!flag[5])prepareGeneralSettings();break;
    }
}


void Settings::preparePrintSettings()
{

    //this fuction is only called once
    flag[0]=true;
    
    QList<int> pos;
    
    //Initializing type1 and cert1
    type1=BONAFIDE;
    cert1="bonafide";
    
    fields[0]<<"serial no"<<"date"<<"student's name"<<"care of"<<"fathers name"<<"roll number"
            <<"course details"<<"from year"<<"to year"<<"purpose text"<<"purpose";
    
    fields[1]<<"serial no"<<"date"<<"student's name"<<"care of"<<"father's name"<<"roll number"
            <<"course details"<<"academic year";
    
    fields[2]<<"Certificate type(original or duplicate)"<<"roll number"<<"admission number"<<"date"
            <<"student's name"<<"father's name"<<"date of birth"<<"date of admission"<<"course"<<"branch"
            <<"date of leaving"<<"qualified"<<"dues"<<"disciplinary measures"
            <<"conduct"<<"community"<<"remarks";
    

    insertFields();
    
}


void Settings::decideCert1( )
{
    QString previous=cert1;
    cert1=sender()->objectName();
    cert1.chop(3);
    
    if(!(previous==cert1))
    {
        if(cert1=="bonafide")
        {
            type1=BONAFIDE;
            paperLabel->setText("A5 Sheet(Land Scape) DIMENSIONS: 210MM X 148MM");
        }
        else if(cert1=="conduct")
        {    type1=CONDUCT;
            paperLabel->setText("A5 Sheet(Land Scape) DIMENSIONS: 210MM X 148MM");
        }
        else
        {
            type1=TC;
            paperLabel->setText("A4 Sheet(Land Scape) DIMENSIONS: 420MM X 296MM");
        }
        insertFields();
    }

}


void Settings::insertFields()
{
    disconnect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setXY(int)));

    fieldCombo->clear();//to avoid append of insertion during revisit
    fieldCombo->insertItems(0,fields[type1]);

    connect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setXY(int)));

    setXY(0);//setting first fields X,Y for view
}

void Settings::setXY(int index)
{
    index*=2;
    settings->beginGroup("certificate/"+cert1+"/positions");

    xSB->setValue(settings->value((QString().setNum(index))).toInt());
    ySB->setValue(settings->value((QString().setNum(index+1))).toInt());
    settings->endGroup();

}


void Settings::savePrintPositions()
{
    f[0]=true;//to detect to generate signal to intimate change
    settings->beginGroup("certificate/"+cert1+"/positions");
    settings->setValue(QString().setNum(2*fieldCombo->currentIndex()),xSB->value());
    settings->setValue(QString().setNum(2*fieldCombo->currentIndex()+1),ySB->value());
    settings->endGroup();
    settings->sync();
}



void Settings::prepareDatabaseSettings()
{
    flag[1]=true;

    //fetch Database Settings

    adminPasswordLE->setFocus();


}

void Settings::adminAuthentication()
{
    QString passwd=adminPasswordLE->text();
    QCryptographicHash hashed(QCryptographicHash::Sha1);
    hashed.addData (QVariant(passwd).toByteArray());


    if (hashed.result()==settings->value("logins/admin").toByteArray())
    {
        databaseGB->setEnabled(true);
        changePasswordGB->setEnabled(true);
        logGB->setEnabled(true);
        //fill interface same as undo for first time

        pLabel->hide();
        pLabel2->hide();
        pLabel3->hide();
        picon->hide();

        if(flag[1])
        {
            getSettings->getDatabaseDetails(databaseDetails);
            undoDatabaseSettings();//same as fill for first time
        }

        if(flag[3])
            fillLogSettings();

    }
    else
    {
        QMessageBox::critical(this,"Authentication Manager","   Incorrect Password !!");
        adminPasswordLE->clear();
    }

}

void Settings::undoDatabaseSettings()
{
    databasenameLE->setText(databaseDetails.at(0));
    hostnameLE->setText(databaseDetails.at(1));
    portLE->setText(databaseDetails.at(2));
    usernameLE->setText(databaseDetails.at(3));
    passwordLE->setText(databaseDetails.at(4));

    undoButton->setEnabled(false);
    reconnectButton->setEnabled(false);

}


void Settings::enableUndoReconnectButtons()
{
    undoButton->setEnabled(true);
    reconnectButton->setEnabled(true);
}

void Settings::prepareAccountSettings()
{
    flag[2]=true;
    adminPasswordLE->setFocus();
}

void Settings::on_newPasswordLE_textEdited()
{
    if (!newPasswordLE->text().isEmpty())
        changeButton->setEnabled(true);
    else
        changeButton->setEnabled(false);
}

void Settings::on_changeButton_clicked()
{
    if (newPasswordLE->text()==confirmPasswordLE->text() && !newPasswordLE->text().isEmpty())
    {

        QCryptographicHash hashed(QCryptographicHash::Sha1);

        hashed.addData(QVariant(confirmPasswordLE->text()).toByteArray());

        settings->setValue("logins/admin",hashed.result());

        if (settings->status()!= QSettings::NoError)
        {
            QMessageBox::warning(this,tr("Password Manager"),tr("Password could not be Changed contact Admin software Dept !!"));
        }
        else
        {
            QMessageBox::information(this,"Password Manager","Password Changed !!");

        }
    }

    else
    {
        QMessageBox::information(this,"Password Manager","Passwords do not match");
        newPasswordLE->clear();
        confirmPasswordLE->clear();
        newPasswordLE->setFocus();
    }

}
void Settings::prepareLogSettings()
{
    qDebug()<<"log";
    flag[3]=true;
    adminPasswordLE->setFocus();
}

void Settings::fillLogSettings()
{
    logPathLE->setText(settings->value("log/backuplogpath").toString());

}

void Settings::saveLogPath()
{
    settings->setValue("log/backuplogpath",logPathLE->text());
}


void Settings::on_browseButton_clicked()
{

    QFileDialog::Options options = QFileDialog::ShowDirsOnly;
    options |= QFileDialog::DontUseNativeDialog;
    directory= QFileDialog::getExistingDirectory(this,
                                                 tr("Choose a folder to store logs"),
                                                 logPathLE->text(),
                                                 options);
    if(!directory.endsWith("/"))
        directory+="/";

    if (!directory.isEmpty())
        logPathLE->setText(directory);

    settings->setValue("log/backuplogpath",directory);

}

void Settings::on_backupButton_clicked()
{
    directory=logPathLE->text();
    QStringList logpath,logdir;
    QString path,dir;
    logpath<<"./logs/certificate/"<<"./logs/session/";
    logdir<<"certificate"<<"session";
    for(int k=0;k<2;k++)
    {qDebug()<<"\n\n\nvalue of k="<<k;
        path=logpath[k];
        dir=logdir[k];

        if(!QDir(directory+dir).exists())
        {
            QDir().mkdir(directory+dir);//create a directory if dir doesn't exist
        }

        QDir logDir(path);

        QStringList files = logDir.entryList(QStringList("*.Log"),QDir::Files,QDir::Time|QDir::Reversed);
        uint size=files.count();


        QProgressDialog progressDialog(0);
        progressDialog.setCancelButtonText(tr("&Cancel"));
        progressDialog.setRange(0, size);
        progressDialog.setWindowTitle(tr("Log Manager Performing Back Up...."));

        QFile logfile;//holds each file for backup

        for (uint i=0;i<size;i++)
        {
            progressDialog.setValue(i);
            progressDialog.setLabelText(tr("Back up file number %1 of %2...\n\nCurrent File: %3")
                                        .arg(i).arg(size).arg(files[i]));
            qApp->processEvents();

            if (progressDialog.wasCanceled())
                break;
            if(!createServerFile( files[i],path,dir))
            {   k=1;//to stop back up of session logs incase of failure with certificate logs
                break;
            }
        }

    }

    if(QFile().error()==QFile::CopyError)
    {

        QMessageBox::information(this,tr("Log Manager"),tr("Back Up failed due to an error.\n\nOperation Aborted !!"),QMessageBox::Ok);

    }
    //else

}


bool Settings::createServerFile(const QString& filename,const QString &path,const QString &dir)
{
    qDebug()<<"input"<<filename<<path;
    bool ok;
    QString localfile=path+filename;

    ok=QFile::copy( localfile,directory+dir+"/"+filename);//copying to central repository

    if (ok)
    {

        qDebug()<<"copy sucesss"<<QFile::rename( localfile,localfile+"c");
        return true;
    }

    else if(QFile().error()==(QFile::CopyError||QFile::TimeOutError||QFile::UnspecifiedError))
    {
        qDebug()<<"iam the error"<<filename<<path<<dir;
        return false;
    }

    return true;
}

void Settings::prepareCertificateSettings()
{
    flag[4]=true;
    cert2="bonafide";
    bonafideRB2->setChecked(true);
    dateEdit->setDate(settings->value("certificate/date").toDate());
    fromSB->setValue(settings->value("certificate/from").toInt());
    toSB->setValue(settings->value("certificate/to").toInt());
}

void Settings::decideCert2()
{

    QString previous=cert2;
    cert2=sender()->objectName();
    cert2.chop(3);
    if(!(cert2==previous))
    {
        fillSerial();

    }
}

void Settings::fillSerial()
{
    settings->beginGroup("certificate/"+cert2);

    serialSB->setValue(settings->value("serialno").toInt());

    settings->endGroup();
}


void Settings::saveCertificateSettings()
{
    f[1]=true;

    settings->beginGroup("certificate/"+cert2);

    settings->setValue("serialno",serialSB->value());
    settings->setValue("date",dateEdit->date().toString("dd.MMM.yyyy"));

    QList<int> fromto;
    getSettings->getDateAcademicYear(fromto);
    fromSB->setValue(fromto.at(0));
    toSB->setValue(fromto.at(1));
    settings->endGroup();
    settings->sync();
}

void Settings::saveAcademicYear()
{
    f[4]=true;
    settings->setValue("general/from",fromSB->value());
    settings->setValue("general/to",toSB->value());
    settings->sync();
}


void Settings::prepareGeneralSettings()
{
    flag[5]=true;

    databaseRB->setChecked(settings->value("general/mode").toBool());

    QList<QPrinterInfo> plist;
    QPrinterInfo pinfo;
    plist=pinfo.availablePrinters();

    QStringList printers;
    for(int i=0;i<plist.size();i++)
        printers<<plist[i].printerName();

    QString temp,dprinter=pinfo.defaultPrinter().printerName();
    temp=dprinter;
    temp+="(Default)";
    printers.replace(printers.indexOf(dprinter),temp);


    if(!printers.isEmpty())
    {
        printersCombo->insertItems(0,printers);
        savePrinter(printers.at(0));
    }

    if( printers.isEmpty())
        QMessageBox::information(this,"Settings Manager","No Printers Currently Connected !! Print in PDF format.");

    else if(printersCombo->findText(getSettings->getDefaultPrinter(),Qt::MatchExactly)==-1 )
        QMessageBox::information(this,"Settings Manager","Default Printer Set to "+printers.at(0)+".\n\nDefault Printer can be changed at Settings->General Settings.");

}

void Settings::saveMode()
{
    if(settings->value("general/mode","true").toBool()!=databaseRB->isChecked())
    {
        f[2]=true;
        settings->setValue("general/mode",databaseRB->isChecked());
    }
}

void Settings::savePrinter(const QString &printer)
{
    f[3]=true;
    settings->setValue("general/defaultprinter",printer);
    settings->sync();
}



void Settings::on_exportButton_clicked()
{
    stringType<<"date"<<"backuplogpath"<<"printer";
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,tr("Export Settings"), QDir::homePath ()+"/Desktop"+"/DocmaQ Settings.drf", tr("DocmaQ Registration Files (*.drf)"),&selectedFilter,options);
    if(!fileName.endsWith("drf"))
        fileName+=".drf";

    if (!fileName.isEmpty())
    {
        file.setFileName(QDir().absoluteFilePath(fileName));
        out.setDevice(&file);
        file.open(QIODevice::WriteOnly|QIODevice::Text);
        out<<"Windows Registry Editor Version 5.00\n"
                <<"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency]\n"
                <<"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\DocmaQ]\n"
                <<"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\DocmaQ\\certificate]\n";

        generateRegFile("certificate","[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\DocmaQ\\certificate");

        out<<"[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\DocmaQ\\general]\n";

        generateRegFile("general","[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\DocmaQ\\general");

        generateRegFile("log","[HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\DocmaQ\\log");
    }
    file.close();

}


void Settings::generateRegFile(QString key,QString path)
{
    //for childkeys
    settings->beginGroup(key);
    QStringList list=settings->childKeys();

    short size= list.size();
    for(short i=0;i<size;i++)
    {
        QString type="dword:";
        QString childkey=list.at(i);
        QString value=settings->value(list.at(i)).toString();
        if(stringType.contains(childkey,Qt::CaseInsensitive))
        {
            type="";
            value.prepend("\"");
            value.append("\"");
        }

        childkey.prepend("\"");
        childkey.append("\"");

        childkey+="="+type+value;
        out<<childkey+"\n";

    }

    list.clear();

    //for child groups
    list=settings->childGroups();

    size= list.size();

    for(short j=0;j<size;j++)
    {

        path+="\\"+list.at(j);

        out<<path+"]\n\n";

        file.flush();
        generateRegFile(list.at(j),path);//recursive call

        short index=path.lastIndexOf("\\");

        path=path.remove(index,path.length()-(index));

    }
    settings->endGroup();
}


void Settings::on_importButton_clicked()
{
    fileName.clear();
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;

    fileName = QFileDialog::getOpenFileName(this,tr("Import Settings"), QDir::homePath ()+"/Desktop", tr("DocmaQ Settings (*.drf)"),&selectedFilter,options);
    QString newname=fileName;
    newname.chop(4);newname+=".reg";
    qDebug()<<"rename"<<QFile::rename(fileName,newname);


    if (!fileName.isEmpty())
        QDesktopServices::openUrl(QUrl::fromLocalFile(QDir().absoluteFilePath(newname)));

    qDebug()<<"end of func";

}


void Settings::closeEvent(QCloseEvent *e)
{
    settings->setValue("general/laststackwidget",stackedWidget->currentIndex());

    QString newname=fileName;
    newname.chop(4);
    newname+=".reg";

    if(QFile::exists(newname))
    {
        qDebug()<<"old"<<newname;
        if(!QFile::rename(newname,fileName))
            QFile::remove(newname);
    }

    for(int i=0;f[i]&&i<5;i++)
        emit sendSignal(i);

}

Settings::~Settings()
{

}
