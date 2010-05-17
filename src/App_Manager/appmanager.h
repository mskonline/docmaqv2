#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QSplashScreen>
#include <QButtonGroup>
#include "../Data_Structures/student.h"


class Interface;
class Student;
class CDatabase;
class CPrinter;
class AuthenDialog;
class logs;
class GetSettings;
class Settings;

class AppManager: public QObject
{
     Q_OBJECT

private :
Interface *interface;
CDatabase *cdb;
CPrinter *cprinter;
AuthenDialog  *ad;
logs *log;
GetSettings *settings;

QSplashScreen *splash;
QButtonGroup *bg;
QString roll,type;
int st_ptr,fg_rollLe;
void format_course();

public:
Student *student;
QList <Student* > st_plist;
AppManager();
void load_init_modules();
void manualProcess();
void previewLastRoll();

int ctype[3];
~AppManager();

public slots:
void print();
void printc(int);
void itemchanged(int ,const QString &);
void btableclicked(int,int);
void topdf();
void onRollEntry();
void updatetype(int);
void load_final_modules();
void removeSt(int);
void rollfocus();
void toLeft();
void toRight();
void updatevalues(int);
};

#endif // APPMANAGER_H
