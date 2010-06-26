#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QSplashScreen>
#include <QButtonGroup>
#include <QTableWidgetItem>

#define FRAME_COLOR "background-color: rgb(203, 203, 203);"
#define BTABLE_FRAME_COLOR "background-color: rgb(255, 237, 132);"

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

public:
Student *student,*tc_student;
QList <Student* > *st_plist;
QList <int> c_sno, c_count;
QStringList dlist;
QString temp;
int c_type[2];

AppManager();
void load_init_modules();
void init_print(int);
void manualProcess();
void previewLastRoll();
void update_counts(int,int);
~AppManager();

private :
Interface *interface;
CDatabase *cdb;
CPrinter *cprinter;
AuthenDialog  *ad;
logs *log;
GetSettings *settings;
QSplashScreen *splash;
QButtonGroup *bg_ctype,*bg_ptype;
QString roll,rtype,btype;
int st_ptr,fg_rollLe,c_mode,d_mode,p_type,et;
int p_lock;
void format_course();


public slots:

void print();
void b_print();
void c_print();
void TC_mode();
void Tc_dissue();
void pcomplete(int);
void update_print_type(int);
void itemchanged(int ,const QString &);
void modechange(int);
void btableclicked(int,int);
void topdf();
void onRollEntry();
void updatetype(int);
void load_final_modules();
void removeSt(int);
void rollfocus();
void toLeft();
void toRight();
void toup();
void todown();
void updatevalues(int);
void quit();
};

#endif // APPMANAGER_H
