/* DocmaQ v2.0, Credential Publishing System
    Copyright (C) 2010 M.Sai Kumar <msk.mymails@gmail.com>

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

#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QSplashScreen>
#include <QButtonGroup>
#include <QTableWidgetItem>

#define FRAME_COLOR ""
#define BTABLE_FRAME_COLOR "background-color: rgb(197,239,253);"

class Interface;
class Student;
class CDatabase;
class CPrinter;
class AuthenDialog;
class d_log;
class GetSettings;

class AppManager: public QObject
{
     Q_OBJECT

public:
    Student *student,*tc_student;
    QList <Student* > *st_plist;
    QList <int> c_sno, c_count;
    QStringList dlist,dblist;
    QString temp;
    int c_type[2],vpic,admn;
    bool d_mode;

    AppManager();
    void load_init_modules();
    void init_print(int);
    void update_counts(int,int);
    ~AppManager();

private :
    Interface *interface;
    CDatabase *cdb;
    CPrinter *cprinter;
    AuthenDialog  *ad;
    d_log *lg;
    GetSettings *settings;
    QSplashScreen *splash;
    QButtonGroup *bg_ctype,*bg_ptype;
    QString roll,rtype,btype;
    int st_ptr,fg_rollLe,c_mode,p_type,et;
    int r_sno[2];

    void recalculate_csno(int);
    void modechange();
    void relogin();

public slots:

    void print();
    void b_print();
    void c_print();
    void pdfprint();
    void TC_mode();
    void TC_dissue();
    void tc_type(int);
    void sno_changed(int);

    void tc_pcomplete();
    void pcomplete(int);
    void createLogUI();
    void createSettingsUI();
    void createSessionPage();
    void update_print_type(int);
    void itemchanged(int ,const QString &);
    void btableclicked(int,int);
    void onRollEntry();
    void revertDetails(int);
    void updatetype(int);
    void load_final_modules(int);
    void removeSt(int);
    void rollfocus();
    void toLeft();
    void toRight();
    void toup();
    void todown();
    void updatevalues(QList <int> &);
    void userchange(int);
    void quit();
};

#endif // APPMANAGER_H
