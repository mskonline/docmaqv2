#include "getsettings.h"
#include "settings.h"
#include <QList>
#include <QStringList>
#include <QSettings>
#include <QtAlgorithms>
#include <QDebug>


GetSettings::GetSettings()
{
    settings=new QSettings(QSettings::SystemScope,"Qogency","DocmaQ");

}


QString GetSettings::decideType(short type)
{
    QString str1;
    switch (type)
    {
    case BONAFIDE:
        str1="bonafide";
        break;
    case CONDUCT :
        str1="conduct";
        break;
    case TC      :
        str1="tc";
        break;
    }
    return str1;
}

QStringList GetSettings::getChildGroups(short type)
{
    QString cert1=decideType(type);
    settings->beginGroup("certificate/"+cert1+"/fields");

    QStringList fieldkeys=settings->childGroups();
    qDebug()<<fieldkeys;
    settings->endGroup();
    return fieldkeys;
}

QStringList GetSettings::getDatabaseDetails()
{
    settings->beginGroup("Database");
    databaseDetails<<settings->value("databasename").toString();
    databaseDetails<<settings->value("hostname").toString();
    databaseDetails<<settings->value("port").toString();
    databaseDetails<<settings->value("username").toString();
    databaseDetails<<settings->value("password").toString();
    settings->endGroup();
    return databaseDetails;
}

QList <int> GetSettings::getCount(short type)
{
    QString str1=decideType(type);
    QList <int> count;

    settings->beginGroup("certificate/"+str1+"/count");
    QStringList list=settings->childKeys();

    short size=list.size();
    for(short i=0;i<size;i++)
    {
        count<<settings->value(list.at(i),0000).toInt();

    }
    settings->endGroup();
    return count;

}

QStringList GetSettings::getLastSnoRollNo(short type)
{
    QString str1=decideType(type);
    QStringList snorollno;

    settings->beginGroup("certificate/"+str1+"/");
    QStringList list;
    list<<"serialno"<<"lastrollno";

    short size=list.size();

    for(short i=0;i<size;i++)
    {
        snorollno<<settings->value(list.at(i),0000).toString();

    }
    settings->endGroup();

    return snorollno;
}


//for every print this is called
void GetSettings::setLastSnoRollNo(short serialno ,QString rollno,short type)
{
    QString str1=decideType(type);
    QStringList snorollno;
    snorollno<<QString().setNum(serialno)<<rollno;

    settings->beginGroup("certificate/"+str1+"/");
    QStringList list;
    list<<"serialno"<<"lastrollno";

    short size=snorollno.size();

    for(short i=0;i<size;i++)
    {
        settings->setValue(list.at(i),snorollno.at(i));

    }

    settings-> endGroup();
    settings->sync();
}

QList<int> GetSettings::getAcademicYear()
{
    QList<int> academicyear;
    academicyear<<settings->value("general/from").toInt();
    academicyear<<settings->value("general/to").toInt();
    return academicyear;
}

QList<int> GetSettings::getPrintPositions(short type)
{
    QList<int> pos,pos1,pos2;

    QStringList fieldkeys=getChildGroups(type);

    short size=fieldkeys.size();

    for(short i=0;i<size;i++)
    {
        pos1<<settings->value(fieldkeys.at(i)+"/id").toInt();
        pos2<<settings->value(fieldkeys.at(i)+"/x").toInt();
        pos2<<settings->value(fieldkeys.at(i)+"/y").toInt();
    }
    for(int i=0;i<size;i++)
    {
        int index=pos1.indexOf(i);
        pos<<pos2.at(2*index);
        pos<<pos2.at(2*index+1);
    }
    settings->endGroup();
    return pos;
}

QList<int> GetSettings::getOrder()
{
    QList<int> order;
    settings->beginGroup("certificate");
    order<<settings->value("bonafide/order").toInt();
    order<<settings->value("conduct/order").toInt();
    order<<settings->value("tc/order").toInt();
    settings->endGroup();
    return order;
}



QStringList GetSettings::getPrinters()
{
    QStringList list;
    //to be done

    return list;
}

QString GetSettings::getDefaultPrinter()
{
    return settings->value("general/defaultprinter").toString();
}

bool GetSettings::isFullScreen()
{
    if(settings->value("general/fullscreen").toString()=="true")
        return true;
    return false;

}

short GetSettings::getTheme()
{
    return settings->value("general/theme").toInt();
}


void  GetSettings::createSettingsInterface()
{

    Settings *st = new Settings(settings,this);

}
