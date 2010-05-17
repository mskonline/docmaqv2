#include "settings.h"
#include "getsettings.h"

#include <QDebug>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QProcess>
#include <QCloseEvent>
#include <QItemSelectionModel>

Settings::Settings(QSettings *settings,GetSettings* getSettings,QWidget *parent)
    : QWidget(parent)
{

    //gets limited settings for interface fill up
    this->getSettings=getSettings;
    this->settings = settings;

    setupUi(this);
    show();

    connect(adminPasswordLE,SIGNAL(returnPressed()),this,SLOT(adminAuthentication()));

    //  setWindowFlags(Qt::FramelessWindowHint);


    //connecting signals
    connect(stackedWidget,SIGNAL(currentChanged(int)),this,SLOT(selectFunction(int)));

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


    //Account page



    //Certificate page

    connect(bonafideRB,SIGNAL(clicked()),this,SLOT(prepareCertificateSettings()));
    connect(conductRB,SIGNAL(clicked()),this,SLOT(prepareCertificateSettings()));
    connect(tcRB,SIGNAL(clicked()),this,SLOT(prepareCertificateSettings()));

    connect(serialSB,SIGNAL(editingFinished()),this,SLOT(saveCertificateSettings()));
    connect(dateEdit,SIGNAL(editingFinished()),this,SLOT(saveCertificateSettings()));



    //print page

    connect(bonafideRB2,SIGNAL(clicked()),this,SLOT(preparePrintSettings()));
    connect(conductRB2,SIGNAL(clicked()),this,SLOT(preparePrintSettings()));
    connect(tcRB2,SIGNAL(clicked()),this,SLOT(preparePrintSettings()));
    connect(fieldCombo,SIGNAL(currentIndexChanged(QString)),this,SLOT(setXY(QString)));
    connect(xSB,SIGNAL(editingFinished()),this,SLOT(savePrintPositions()));
    connect(ySB,SIGNAL(editingFinished()),this,SLOT(savePrintPositions()));

    //Log settings
    connect(logDurationLE,SIGNAL(editingFinished()),this,SLOT(saveLogDuration()));

    //General Settings
    //connect(fullScreenCB,toggled(bool),appManager,SLOT(fullscreen()));
    connect(fullScreenCB,SIGNAL(toggled(bool)),this,SLOT(saveFullscreen(bool)));
    connect(themeCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(saveTheme(int)));
    connect(printersCombo,SIGNAL(currentIndexChanged( const QString &)),this,SLOT(savePrinter(const QString)));
    connect(fromSB,SIGNAL(editingFinished()),this,SLOT(saveAcademicYear()));
    connect(toSB,SIGNAL(editingFinished()),this,SLOT(saveAcademicYear()));
    qDebug()<<"wats";
    listWidget->setCurrentRow(3,QItemSelectionModel::ToggleCurrent);
//settings->value("general/laststackwidget",0).toInt()
}
void Settings::selectFunction(int id)
{
    qDebug()<<"gfhfghgfhfgh";
    switch(id)
    {
    case 0:if(!flag[0])preparePrintSettings();
    case 1:if(!flag[1])prepareDatabaseSettings();
    case 2:if(!flag[2])prepareAccountSettings();
    case 3:if(!flag[3])prepareLogSettings();
    case 4:if(!flag[4])prepareCertificateSettings();
    case 5:if(!flag[5])prepareGeneralSettings();
    }
}


void Settings::decideCert2( short &type)
{

    if(bonafideRB2->isChecked())
        type=BONAFIDE;

    else if(conductRB2->isChecked())
        type=CONDUCT;
    else
        type=TC;

}

void Settings::preparePrintSettings()
{

    short type;

    flag[0]=true;
    fieldCombo->clear();

    decideCert2(type);

    QList<int> pos=getSettings->getPrintPositions(type);

    short size=pos.size();

    QStringList fieldkeys=getSettings->getChildGroups(type);

    for(short i=0;i<size;i=i+3)
    {          qDebug()<<"hello";
        fieldCombo->insertItem(pos.at(i),fieldkeys.at(i));
    }
    settings->endGroup();

    //for displaying first entry
    setXY(fieldkeys.at(0));

    if(type==BONAFIDE ||type==CONDUCT)
        paperLabel->setText("A5 Sheet(Land Scape) DIMENSIONS: 210MM X 148MM");
    else
        paperLabel->setText("A4 Sheet(Land Scape) DIMENSIONS: 420MM X 296MM");
    error();
}

void Settings::setXY(QString text)
{
    f[0]=true;
    settings->beginGroup("certificate/"+cert1+"/fields/"+text);
    xSB->setValue(settings->value("x").toInt());
    ySB->setValue(settings->value("y").toInt());
    settings->endGroup();
    error();
}


void Settings::savePrintPositions()
{
    f[0]=true;
    QString text= fieldCombo->currentText();
    settings->beginGroup("certificate/"+cert1+"/fields/"+text);
    settings->setValue("x",xSB->value());
    settings->setValue("y",ySB->value());
    settings->endGroup();
    error();
}



void Settings::prepareDatabaseSettings()
{
    flag[1]=true;
    qDebug()<<"prepareDatabaseSettings";

    //fetch Database Settings

    databaseDetails=getSettings->getDatabaseDetails();

    error();
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

        pLabel->clear();
        pLabel2->clear();
        undoDatabaseSettings();
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
    error();
}


void Settings::enableUndoReconnectButtons()
{
    undoButton->setEnabled(true);
    reconnectButton->setEnabled(true);
}

void Settings::prepareAccountSettings()
{
    flag[2]=true;

}

void Settings::on_newPasswordLE_editingFinished()
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
    error();
}
void Settings::prepareLogSettings()
{
    flag[3]=true;
}

void Settings::fillLogSettings()
{
    settings->beginGroup("log");

    if (settings->value("logmode","local")=="local")
        localCB->setChecked(true);
    else
        centralCB->setChecked(true);

    logDurationLE->setText(settings->value("logduration",360).toString());
    logPathLE->setText(settings->value("centrallogpath").toString());
    settings->endGroup();

}
void Settings::saveLogDuration()
{
    settings->setValue("log/logduration",logDurationLE->text());
}




void Settings::on_browseButton_clicked()
{

    QFileDialog::Options options = QFileDialog::ShowDirsOnly;
    options |= QFileDialog::DontUseNativeDialog;
    QString directory = QFileDialog::getExistingDirectory(this,
                                                          tr("Choose a folder to store logs"),
                                                          logPathLE->text(),
                                                          options);
    if (!directory.isEmpty())
        logPathLE->setText(directory);

    settings->setValue("log/centrallogpath",directory);

}
void Settings::on_defaultButton_clicked()
{
    settings->beginGroup("log");
    logDurationLE->setText(settings->value("logduration",250).toString());
    logPathLE->setText(settings->value("centrallogpath").toString());
    settings->endGroup();

    localCB->setChecked(true);
    error();
}

void Settings::decideCert1( QString &cert)
{

    if(bonafideRB->isChecked())
        cert=bonafideRB->objectName();

    else if(conductRB->isChecked())
        cert=conductRB->objectName();

    else
        cert=tcRB->objectName();


}

void Settings::prepareCertificateSettings()
{
    flag[4]=true;

    bonafideRB->setChecked(true);
    setOrder();

    QString cert;
    decideCert1(cert);

    cert.chop(2);
    settings->beginGroup("certificate/"+cert);
    serialSB->setValue(settings->value("serialno").toInt());
    dateEdit->setDate(settings->value("date").toDate());
    settings->endGroup();
    error();
}


void Settings::saveCertificateSettings()
{
    f[3]=true;
    QString cert;
    decideCert1(cert);
    cert.chop(2);
    settings->beginGroup("certificate/"+cert);
    settings->setValue("serialno",serialSB->value());
    settings->setValue("date",dateEdit->date().toString("dd.MMM.yyyy"));
    settings->endGroup();
}

void Settings::setOrder()
{
    QList <int> order=getSettings->getOrder();

    QStringList tabtext,name;
    QString temp;
    tabtext<<"   BONAFIDE "<<"    CONDUCT   "<<"         TC        ";
    name<<"bonafide"<<"conduct"<<"tc";

    for(short i=0;i<3;i++)
    {
        temp=name.at(i);
        tabWidget->widget(order.at(i))->setObjectName(temp+"Tab");
        tabWidget->setTabText(order.at(i),tabtext.at(i));

    }
}

void Settings::saveOrder()
{
    f[1]=true;
    for(short i=0;i<3;i++)
    {
        QString  name=  tabWidget->widget(i)->objectName();
        name.chop(3);
        settings->setValue("certificate/"+name+"/order",i);
    }
}

void Settings::prepareGeneralSettings()
{
    flag[5]=true;
    themeCombo->setCurrentIndex(getSettings->getTheme());
    fullScreenCB->setChecked(getSettings->isFullScreen());
    QList<int> fromto=getSettings->getAcademicYear();
    fromSB->setValue(fromto.at(0));
    toSB->setValue(fromto.at(1));
    getPrinters();
}

void Settings::getPrinters()
{
    QStringList printers=getSettings->getPrinters();
    if( !printers.isEmpty())
        QMessageBox::information(this,"Settings Manager","No Printers Currently Connected !!");

    //else if(printersCombo->findText(getSettings->getDefaultPrinter(),Qt::MatchExactly)==-1 )
      //  QMessageBox::information(this,"Settings Manager","Default Printer Set to "+printers.at(0)+"\n\nDefault Printer Can be changed at Settings->General Settings");

    savePrinter(printers.at(0));
}

void Settings::savePrinter(const QString &printer)
{
    f[2]=true;
    settings->setValue("general/defaultprinter",printer);
    settings->sync();
}



void Settings::saveAcademicYear()
{
    f[4]=true;
    settings->setValue("general/from",fromSB->value());
    settings->setValue("general/to",toSB->value());
    settings->sync();
}

void Settings::on_exportButton_clicked()
{
    stringType<<"date"<<"logpath"<<"theme"<<"fullscreen"<<"logmode";
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,tr("Export Settings"), QDir::homePath ()+"/Desktop"+"/DocmaQ Settings.reg", tr("Registration files (*.reg)"),&selectedFilter,options);
    if(!fileName.endsWith("reg"))
        fileName+=".reg";
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
        generateRegFile(list.at(j),path);

        short index=path.lastIndexOf("\\");

        path=path.remove(index,path.length()-(index));

    }
    settings->endGroup();
}


void Settings::on_importButton_clicked()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;

    QString fileName = QFileDialog::getOpenFileName(this,tr("Import Settings"), QDir::homePath ()+"/Desktop", tr("DocmaQ Settings (*.reg)"),&selectedFilter,options);

    if (!fileName.isEmpty())
        QDesktopServices::openUrl(QUrl::fromLocalFile(QDir().absoluteFilePath(fileName)));

}

inline void Settings::messageBox(char *title,char *message)
{
    QMessageBox::information(this,tr(title),tr(message),
                             QMessageBox::Ok|QMessageBox::Default,
                             QMessageBox::NoButton,QMessageBox::NoButton);

}
inline void Settings::criticalMessageBox(char *title,char *message)
{

    QMessageBox::critical(this,tr(title),tr(message),
                          QMessageBox::Ok|QMessageBox::Default);
    /*
    if(QMessageBox::RestoreDefaults== QMessageBox::critical(this,tr(title),tr(message),
                                                            QMessageBox::RestoreDefaults|QMessageBox::Default,
                                                            QMessageBox::Cancel,QMessageBox::NoButton))
    {
        QProcess *myProcess = new QProcess(this);
        myProcess->start("./loadregmain",NULL);
    }
*/
}

void Settings::error()
{

    if (settings->status()!= QSettings::NoError)
    {

        criticalMessageBox("Settings Manager","\
                           Error occured during fetching the settings.\n\n\
                           Reset all settings to solve the problem.\n\n\
                           CAUTION: Export the settings to save critical settings and Import them after reset.");

    }
}


void Settings::saveTheme(int index)
{
    settings->setValue("general/theme",index);
    emit changeTheme(themeCombo->currentText());
}

void Settings::saveFullscreen(bool f)
{
    if(f)
    settings->setValue("general/fullscreen","true");
    else
    settings->setValue("general/fullscreen","false");
}

void Settings::closeEvent(QCloseEvent *e)
{

    if(f[0])
        emit updatePrintPositions(0);
    if(f[1])
    {
        saveOrder();
        emit updateOrder(1);
    }
    if(f[2])
        emit updatePrinter(2);
    if(f[3])
        emit updateCertificateSettings(3);
    if(f[4])
        emit updateAcademicYear(4);
    if(f[5])
        emit removeExcessLogFiles(5);
    if(f[6])
        emit checkLogDirectory(6);



    settings->setValue("general/laststackwidget",stackedWidget->currentIndex());

    settings->beginGroup("log");
    if (localCB->isChecked())
        settings->setValue("logmode","local");
    else
        settings->setValue("logmode","central");

    settings->endGroup();
}

Settings::~Settings()
{

}
