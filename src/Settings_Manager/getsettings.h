#ifndef GetSettings_H
#define GetSettings_H

#include <QList>
#include <QStringList>
#include <QSettings>
#include <QDebug>

class GetSettings:public QObject
{

Q_OBJECT
public :


enum certificateType{BONAFIDE,CONDUCT,TC};

QStringList databaseDetails;

GetSettings();

QString decideType(short type);

QStringList getChildGroups(short type);

//-----database
QStringList getDatabaseDetails();

//-----certificate
QList <int> getCount(short type);

QStringList getLastSnoRollNo(short type);

//for every print this is called
void setLastSnoRollNo(short serialno ,QString rollno,short type);

//-----general
QList<int> getAcademicYear();

QList<int> getPrintPositions(short type);

QList<int> getOrder();

QStringList getPrinters();

QString getDefaultPrinter();

bool isFullScreen();

short getTheme();

public slots:

void createSettingsInterface();

private:

QSettings *settings;

};




#endif // GetSettings_H
