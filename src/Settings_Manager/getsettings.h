#ifndef GetSettings_H
#define GetSettings_H

#include <QList>
#include <QStringList>
#include <QSettings>
#include <QDebug>
#include <QWidget>


enum certificateType{BONAFIDE,CONDUCT,TC};

class GetSettings:public QObject
{

    Q_OBJECT
    
public :

        QString cert;

GetSettings();
~GetSettings();

void setType(const short& type);

void getFieldGroups(QStringList& fieldkeys);

//-----database
void getDatabaseDetails(QStringList& databasedetails);

//-----certificate
void getSessionInfo(QStringList& info);

void setCountSno();

void getSno(QString& sno);


//-----general
void  getDateAcademicYear(QList<int>& academicyear);

void getPrintPositions(QList<int>& pos,short type);

void  setPrinters(QStringList list);

QString getDefaultPrinter();


public slots:

void createSettingsInterface(int id);

private:

QSettings *settings;
QStringList databaseDetails;

void error1();


};




#endif // GetSettings_H
