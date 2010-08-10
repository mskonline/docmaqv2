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

#include "settings.h"
#include "getsettings.h"
#include <QProgressDialog>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QPrinterInfo>
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QSqlError>

Settings::Settings(int user,QSettings *settings,GetSettings* getSettings,QWidget *parent)
    : QWidget(parent,Qt::Window |Qt::WindowCloseButtonHint)
{
    setupUi(this);

    //gets limited settings for interface fill up
    this->getSettings=getSettings;
    this->settings = settings;

    connect(adminPasswordLE,SIGNAL(returnPressed()),this,SLOT(adminAuthentication()));

    //connecting signals

    connect(okButton,SIGNAL(clicked()),this,SLOT(saveSettings()));
    //print page
    connect(bonafideRB1,SIGNAL(clicked()),this,SLOT(decideCert1()));
    connect(conductRB1,SIGNAL(clicked()),this,SLOT(decideCert1()));
    connect(tcRB1,SIGNAL(clicked()),this,SLOT(decideCert1()));

    connect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setXY(int)));
    connect(xSB,SIGNAL(editingFinished()),this,SLOT(setPrintPositionsFlag()));
    connect(ySB,SIGNAL(editingFinished()),this,SLOT(setPrintPositionsFlag()));

    //database page

    connect(databasenameLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    connect(hostnameLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    connect(portLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    connect(usernameLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    connect(passwordLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));

    connect(databasenameLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableReconnectButton()));
    connect(portLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableReconnectButton()));
    connect(hostnameLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableReconnectButton()));
    connect(usernameLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableReconnectButton()));
    connect(passwordLE,SIGNAL(textEdited(const QString &)),this,SLOT(enableReconnectButton()));

    connect(reconnectButton,SIGNAL(clicked()),this,SLOT(checkConnectivity()));

    //Certificate page

    connect(bonafideRB2,SIGNAL(clicked()),this,SLOT(decideCert2()));
    connect(conductRB2,SIGNAL(clicked()),this,SLOT(decideCert2()));
    connect(tcRB2,SIGNAL(clicked()),this,SLOT(decideCert2()));

    connect(dateEdit,SIGNAL(editingFinished()),this,SLOT(setDateAcademicYearFlag()));
    connect(fromSB,SIGNAL(editingFinished()),this,SLOT(setDateAcademicYearFlag()));
    connect(toSB,SIGNAL(editingFinished()),this,SLOT(setDateAcademicYearFlag()));

    //General Settings
    connect(printersCombo,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(setPrinterFlag( const QString&)));

    connect(databaseRB,SIGNAL(clicked()),this,SLOT(setModeFlag()));
    connect(manualRB,SIGNAL(clicked()),this,SLOT(setModeFlag()));

    int laststackwidget=settings->value("general/laststackwidget",0).toInt();
    listWidget->setCurrentRow(laststackwidget,QItemSelectionModel::ToggleCurrent);

    connect(stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(selectFunction(int)));
    selectFunction(laststackwidget);

    if(user == 0)
        tcRB2->hide();

    //ensuring that all flags are reset
    for(int i = 0; i < 8; ++i )
        f[i] = false;

    for(int i = 0; i < 6; ++i)
        flag[i] = false;

    au = false;
}

void Settings::selectFunction(int id)
{
    //showing authentication only when required
    if(!au)
    {
        if(id==0||id==4||id==5)
        {
            puLabel->hide();
            adminPasswordLE->hide();
        }
        else
        {
            puLabel->show();
            adminPasswordLE->show();
        }
    }

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

    //Initializing type1 and cert1
    type1=BONAFIDE;
    cert1="bonafide";

    fields[0]<<"Serial No."<<"Date"<<"Student's Name"<<"Relation (S/o, D/o)"<<"Father's Name"<<"Roll Number"
            <<"Course Details"<<"From Year"<<"To Year"<<"Purpose Text"<<"Purpose";

    fields[1]<<"Serial No."<<"Date"<<"Student's Name"<<"Relation (S/o, D/o)"<<"Father's Name"<<"Roll Number"
            <<"Course Details"<<"Academic Year";

    fields[2]<<"Serial No."<<"Certificate Type(Original or Duplicate)"<<"Roll Number"<<"Admission Number"<<"Date"
            <<"Student's Name"<<"Father's Name"<<"Date Of Birth"<<"Date Of Admission"<<"Course"<<"Branch"
            <<"Date Of Leaving"<<"Qualified"<<"Dues"<<"Disciplinary Measures"
            <<"Conduct"<<"Community"<<"Remarks";

    paperLabel->setText("A5 Sheet(Land Scape) DIMENSIONS: 210 MM X 148 MM");
    insertFields();
}

void Settings::decideCert1( )
{
    QString previous=cert1;
    cert1=sender()->objectName();
    cert1.chop(3);

    if(!(previous==cert1))
    {
        if(cert1.startsWith("b"))
        {
            type1=BONAFIDE;
            paperLabel->setText("A5 Sheet(Land Scape)  DIMENSIONS: 210 MM X 148 MM");
        }
        else if(cert1.startsWith("c"))
        {
            type1=CONDUCT;
            paperLabel->setText("A5 Sheet(Land Scape)  DIMENSIONS: 210 MM X 148 MM");
        }
        else
        {
            type1=TC;
            paperLabel->setText("A4 Sheet(Portrait)  DIMENSIONS: 210 MM X 297 MM");
        }

        insertFields();
    }
}

void Settings::insertFields()
{
    //one flag is set we need not have the signals as the settings are saved only at ok
    disconnect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setXY(int)));

    fieldCombo->clear();//to avoid append of insertion during revisit
    fieldCombo->insertItems(0,fields[type1]);

    connect(fieldCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(setXY(int)));

    setXY(0);//setting first fields X,Y for view
}

void Settings::setXY(int index)
{
    if(pos[type1].contains(index))
    {
        //take value from the pos list if present there
        xSB->setValue(x[type1].at(pos[type1].indexOf(index)));
        ySB->setValue(y[type1].at(pos[type1].indexOf(index)));
    }
    else
    {
        //take values from  registry
        QString temp;
        settings->beginGroup("certificate/"+cert1+"/positions");
        index*=2;
        xSB->setValue(settings->value((temp.setNum(index)),0).toInt());
        ySB->setValue(settings->value((temp.setNum(index+1)),0).toInt());
        settings->endGroup();
    }
}

void Settings::setPrintPositionsFlag()
{
    int index=fieldCombo->currentIndex();

    if(pos[type1].contains(index))
    {
        //Entries will be made only in case of modification of X,Y
        int ind= pos[type1].indexOf(index);
        x[type1].replace(ind,xSB->value());
        y[type1].replace(ind,ySB->value());
    }
    else
    {
        //enter the index and then values into the list
        pos[type1]<<index;
        x[type1]<<xSB->value();
        y[type1]<<ySB->value();
    }

    //to generate signal and intimate change at close
    if(cert1.startsWith("b"))
        f[4]=true;
    else if(cert1.startsWith("c"))
        f[5]=true;
    else
        f[6]=true;
}

void Settings::prepareDatabaseSettings()
{
    flag[1]=true;
    adminPasswordLE->setFocus();
}

void Settings::adminAuthentication()
{
    QString passwd=adminPasswordLE->text();

    if(passwd.isEmpty())
        return;

    QCryptographicHash hashed(QCryptographicHash::Sha1);
    hashed.addData (QVariant(passwd).toByteArray());

    QSettings *settings1=new QSettings(QSettings::SystemScope,"DocmaQ","App");
    if (hashed.result()==settings1->value("logins/admin").toByteArray())
    {
        databaseGB->setEnabled(true);
        changePasswordGB->setEnabled(true);
        logGB->setEnabled(true);
        userCombo->setEnabled(true);
        //fill interface same as undo for first time

        pLabel->hide();
        pLabel2->hide();
        pLabel3->hide();
        puLabel->hide();
        adminPasswordLE->hide();
        tcRB2->show();

        au = true;

        if(flag[1])
        {
            getSettings->getDatabaseDetails(databaseDetails);
            fillDatabaseSettings();
        }

        if(flag[3])
            fillLogSettings();
    }
    else
    {
        QMessageBox::critical(this,"DocmaQ Authentication","   Incorrect Password !");
        adminPasswordLE->clear();
    }
}

void Settings::fillDatabaseSettings()
{
    databasenameLE->setText(databaseDetails.at(0));
    hostnameLE->setText(databaseDetails.at(1));
    portLE->setText(databaseDetails.at(2));
    usernameLE->setText(databaseDetails.at(3));
    passwordLE->setText(databaseDetails.at(4));

    reconnectButton->setEnabled(false);
}

void Settings::databaseSettingsChanged()
{
    f[7]=true;

    //once flag is set we need not have the signals as the settings are saved only at ok
    disconnect(databasenameLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    disconnect(hostnameLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    disconnect(portLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    disconnect(usernameLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
    disconnect(passwordLE,SIGNAL(editingFinished()),this,SLOT(databaseSettingsChanged()));
}

void Settings::enableReconnectButton()
{
    if(databasenameLE->text().isEmpty()||hostnameLE->text().isEmpty()|| portLE->text().isEmpty()||usernameLE->text().isEmpty())
        reconnectButton->setEnabled(false);
    else
        reconnectButton->setEnabled(true);
}

void Settings::checkConnectivity()
{
    bool ok;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","test");
        db.setDatabaseName(databasenameLE->text());
        db.setPort(portLE->text().toInt());
        db.setHostName(hostnameLE->text());
        db.setUserName(usernameLE->text());
        db.setPassword(passwordLE->text());
        ok=db.open();

        if(ok)
            QMessageBox::information(this,"DocmaQ Connection","Connection Successful. <b>" + usernameLE->text() + "</b> is now Connected to <b>" + databasenameLE->text() + "</b> Database.");
        else
            QMessageBox::critical(this,"DocmaQ Connection Error", "An Error Occured while Connecting to the Database." +
                                  tr("Please Check the Database details.\n\n") + "MySQL Reports : " + db.lastError().databaseText());
        db.close();
    }
    QSqlDatabase::removeDatabase("test");
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
        if(newPasswordLE->text().size()<5)
            QMessageBox::information(this,tr("DocmaQ Password Manager"),tr("Password should atleast have 5 characters !"));

        else
        {
            QCryptographicHash hashed(QCryptographicHash::Sha1);

            hashed.addData(QVariant(confirmPasswordLE->text()).toByteArray());
            QSettings *settings1=new QSettings(QSettings::SystemScope,"DocmaQ","App");

            settings1->setValue("logins/"+userCombo->currentText().toLower(),hashed.result());

            if (settings1->status()!= QSettings::NoError)
                QMessageBox::warning(this,tr("DocmaQ Password Manager"),tr("Password could not be Changed.Please Contact Admin !"));
            else
            {
                QMessageBox::information(this,"DocmaQ Password Manager","Password Changed.");

            }
        }
    }
    else
    {
        QMessageBox::information(this,"DocmaQ Password Manager","Passwords do not match");
        newPasswordLE->clear();
        confirmPasswordLE->clear();
        newPasswordLE->setFocus();
    }

}

void Settings::prepareLogSettings()
{
    flag[3]=true;
    adminPasswordLE->setFocus();
}

void Settings::fillLogSettings()
{
    logPathLE->setFocus();
}

void Settings::on_browseButton_clicked()
{
    QFileDialog::Options options = QFileDialog::ShowDirsOnly;
    directory= QFileDialog::getExistingDirectory(this,
                                                 tr("Choose a folder to store logs"),
                                                 "\\",
                                                 options);

    if(directory.isEmpty())
        return;

    if(!directory.endsWith("/"))
        directory+="/";

    logPathLE->setText(directory);
}


void Settings::on_backupButton_clicked()
{
    if(logPathLE->text().isEmpty())
        return;
    directory=logPathLE->text()+"DocmaQ Log Backup/";
    QStringList logpath,logdir;
    QString path,dir;
    logpath<<"./logs/certificate/"<<"./logs/session/";
    logdir<<"certificate"<<"session";
    int q=0;
    for(int k=0;k<2;k++)
    {
        path=logpath[k];
        dir=logdir[k];

        QDir logDir(path);

        QStringList files = logDir.entryList(QStringList("*.Log"),QDir::Files,QDir::Time|QDir::Reversed);
        uint size=files.count();

        if(!QDir(directory+dir).exists())
        {
            QDir().mkdir(directory);
            QDir().mkdir(directory+dir);//create a directory if dir doesn't exist
        }

        QProgressDialog progressDialog(this);
        progressDialog.setCancelButtonText(tr("&Cancel"));
        progressDialog.setRange(0, size);
        progressDialog.setWindowTitle(tr("Performing Back Up....      "));
        progressDialog.show();

        for (uint i=0;i<size;i++)
        {
            q=1;
            progressDialog.setValue(i);
            progressDialog.setLabelText(tr("Back up file %1 of %2...\n\nCurrent File: %3")
                                        .arg(i).arg(size).arg(files[i]));
            qApp->processEvents();

            if (progressDialog.wasCanceled())
                break;
            if(!createServerFile( files[i],path,dir))
            {
                k=1;//to stop back up of session logs incase of failure with certificate logs
                break;
            }
        }
    }

    if(QFile().error()==QFile::CopyError)
        QMessageBox::information(this,tr("DocmaQ Log Manager"),tr("Back Up failed due to an error.\n\nOperation Aborted !!"),QMessageBox::Ok);
    else
    {
        if(q==1)
            QMessageBox::information(this,tr("DocmaQ Log Manager"),tr("Back Up Successful !!"),QMessageBox::Ok);
    }
}

bool Settings::createServerFile( QString& filename, QString &path, QString &dir)
{
      QString localfile=path+filename;
    QFile::copy( localfile,directory+dir+"/"+filename);//copying to central repository

    if(QFile().error())
        return false;

    return true;
}

void Settings::prepareCertificateSettings()
{
    flag[4]=true;
    cert2="bonafide";
    type2=BONAFIDE;
    bonafideRB2->setChecked(true);

    settings->beginGroup("certificate");

    ser.insert(0,settings->value("bonafide/serialno",0).toInt());
    ser.insert(1,settings->value("conduct/serialno",0).toInt());
    ser.insert(2,settings->value("tc/serialno",0).toInt());

    serialSB->setValue(ser[type2]);
    dateEdit->setDate(settings->value("date","").toDate());
    fromSB->setValue(settings->value("from",0).toInt());
    toSB->setValue(settings->value("to",0).toInt());
    settings->endGroup();
}

void Settings::decideCert2()
{
    QString previous=cert2;
    cert2=sender()->objectName();
    cert2.chop(3);
    serialSB->setMaximum(9999);

    if(cert2.startsWith("b"))
        type2=BONAFIDE;
    else if(cert2.startsWith("c"))
        type2=CONDUCT;
    else
        type2=TC;

    if(!(cert2==previous))
    {
        if(cert2.startsWith("t"))
            serialSB->setMaximum(99999);
    }

    serialSB->setValue(ser[type2]);
}

void Settings::on_serialSB_editingFinished()
{
    f[1]=true;
    ser[type2]=serialSB->value();
}

void Settings::setDateAcademicYearFlag()
{
    f[2]=true;

    //one flag is set we need not have the signals as the settings are saved only at ok
    disconnect(dateEdit,0,this,SLOT(setDateAcademicYearFlag()));
    disconnect(fromSB,0,this,SLOT(setDateAcademicYearFlag()));
    disconnect(toSB,0,this,SLOT(setDateAcademicYearFlag()));
}

void Settings::prepareGeneralSettings()
{
    flag[5]=true;

    //once flag is set we need not have the signals as the settings are saved only at ok
    disconnect(printersCombo,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(setPrinterFlag( const QString&)));
    databaseRB->setChecked(settings->value("general/mode",true).toBool());
    manualRB->setChecked(!settings->value("general/mode",false).toBool());

    QList<QPrinterInfo> plist;
    QPrinterInfo pinfo;
    plist=pinfo.availablePrinters();

    QStringList printers;
    for(int i=0;i<plist.size();i++)
        printers<<plist[i].printerName();
    int index=printers.indexOf( settings->value("general/printer","Unavailable").toString());

    QString temp,dprinter=pinfo.defaultPrinter().printerName();
    temp=dprinter;
    temp+="(Default)";//highlighting default printer
    printers.replace(printers.indexOf(dprinter),temp);
    printersCombo->insertItems(0,printers);//inserting all the printers
    //checking whether the current printer is there in the list newly generated
    //if not there show a msg saying "No printer assigned"

    if(index==-1)
    {
        printersCombo->insertItem(0,"No Printer Assigned");
        QMessageBox::information(this,"DocmaQ Settings","No Printer Assingned.Select a printer from the Available Printers ");
    }
    else
        printersCombo->setCurrentIndex(index);

    connect(printersCombo,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(setPrinterFlag( const QString&)),Qt::UniqueConnection);
}

void Settings::setModeFlag()
{
    f[0]=true;

    //once flag is set we need not have the signals as the settings are saved only at ok
    disconnect(databaseRB,SIGNAL(clicked()),this,SLOT(setModeFlag()));
    disconnect(manualRB,SIGNAL(clicked()),this,SLOT(setModeFlag()));
}

void Settings::setPrinterFlag( const QString &printer)
{
    f[3]=true;

    //once flag is set we need not have the signals as the settings are saved only at ok
    disconnect(printersCombo,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(setPrinterFlag( const QString&)));
}

void Settings::on_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Export Settings"), QDir::homePath ()+"/Desktop"+"/DocmaQ Settings.drf", tr("DocmaQ Registration Files (*.drf)"));

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(".drf"))
        fileName+=".drf";

    QProcess::execute("regedit.exe",QStringList()<<"/e"<<fileName<<"HKEY_LOCAL_MACHINE\\SOFTWARE\\Qogency\\");
    QMessageBox::information(this,"DocmaQ Settings","Settings Successfully Exported");
}

void Settings::on_importButton_clicked()
{
    fileName.clear();
    fileName = QFileDialog::getOpenFileName(this,tr("Import Settings"), QDir::homePath ()+"/Desktop", tr("DocmaQ Settings (*.drf)"));

    if (!fileName.isEmpty())
    {
        QProcess ::execute("regedit.exe",QStringList()<<"/s"<<fileName);
        QMessageBox::information(this,"DocmaQ Settings","Settings Successfully Imported");
    }
}

void Settings::saveSettings()
{
    QString cert;
    bool z = false,willclose=true;
    int type;
    siglist.clear();

    for(int i=0;i<8;i++)
    {
        if(f[i])
        {
            switch(i)
            {
            case 7://for saving db tab details
                if(databasenameLE->text().isEmpty()||hostnameLE->text().isEmpty()||usernameLE->text().isEmpty())
                {
                    willclose=false;
                    QMessageBox::warning(this,tr("DocmaQ Settings   "),tr("The Database Details has some unspecified fields.Please fill them with valid details inorder to connect to database."));
                }
                else
                {
                    settings->setValue("Database/databasename",databasenameLE->text());
                    settings->setValue("Database/hostname",hostnameLE->text());
                    settings->setValue("Database/port",portLE->text());
                    settings->setValue("Database/username",usernameLE->text());
                    settings->setValue("Database/password",passwordLE->text());
                }
                if(!z)
                {
                    siglist << 0;
                    z = true;
                }
                break;

                //for saving mode
            case 0: settings->setValue("general/mode",databaseRB->isChecked());
                   settings->sync();

                   if(!z)
                   {
                        siglist << 0;
                        z = true;
                   }
                   break;

            case 1://for saving certificate tab details

                settings->beginGroup("certificate");
                settings->setValue("bonafide/serialno",ser[0]);
                settings->setValue("conduct/serialno",ser[1]);
                settings->setValue("tc/serialno",ser[2]);
                settings->endGroup();
                siglist << 1;
                break;

            case 2://for date and academic year
                settings->setValue("certificate/date",dateEdit->date());
                settings->setValue("certificate/from",fromSB->value());
                settings->setValue("certificate/to",toSB->value());
                settings->sync();
                siglist << 2;
                break;

            case 3://for saving the printer
                if(printersCombo->currentText().contains("(Default)"))
                    settings->setValue("general/printer",printersCombo->currentText().replace("(Default)",""));
                else
                    settings->setValue("general/printer",printersCombo->currentText());
                siglist << 3;
                break;

            case 4://for saving print positions for bonafide
                cert="bonafide";
                type=0;
                savep(cert,type);
                siglist <<4;
                break;

            case 5://for saving print positions for conduct
                cert="conduct";
                type=1;
                savep(cert,type);
                siglist << 5;
                break;

            case 6://for saving print positions for tc
                cert="tc";
                type=2;
                savep(cert,type);
                siglist << 6;
                break;

            }
        }
    }

    if(willclose)
        close();      
}

void Settings::savep(QString& cert,int &type)
{
    if(!pos[type].isEmpty())
    {
        QString temp;

        int size=pos[type].size();

        for(int j=0;j<size;j++)
        {
            int index=pos[type].at(j);
            index*=2;
            settings->beginGroup("certificate/"+cert+"/positions");
            settings->setValue(temp.setNum(index),x[type].at(j));
            settings->setValue(temp.setNum(index+1),y[type].at(j));
            settings->endGroup();
        }
    }
}

void Settings::closeEvent(QCloseEvent *e)
{
    settings->setValue("general/laststackwidget",stackedWidget->currentIndex());
    emit sendSignal(siglist);
}

Settings::~Settings()
{
}

