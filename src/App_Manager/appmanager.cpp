#include "appmanager.h"
#include "../Database_Manager/cdatabase.h"
#include "../interface/interface.h"
#include "../Print_Manager/cprinter.h"
#include "../Authentication_Manager/authendialog.h"
#include "../Settings_Manager/settings.h"
#include "../Settings_Manager/getsettings.h"
#include "../Log_Manager/log.h"


#include <QDebug>

#define FRAME_COLOR "background-color: rgb(203, 203, 203);"
#define BTABLE_FRAME_COLOR "background-color: rgb(255, 237, 132);"

AppManager::AppManager()
{
    /*splash = new QSplashScreen;
    splash->setPixmap(QPixmap("./stpic.png"));
    splash->show();



    Qt::Alignment bottomleft = Qt::AlignLeft | Qt::AlignBottom;
    splash->showMessage(QObject::tr("Initializing..."),bottomleft, Qt::white);*/

    st_ptr = 0;
    load_init_modules();
    ctype[0] = 1;
    ctype[1] = 0;
    ctype[2] = 0;
}

void AppManager::load_init_modules()
{
    // Load Settings, Auth, DB
    cdb = new CDatabase();
    //ad=new AuthenDialog();
    //ad->show();

    log= new logs(new QTextEdit);
    settings=new GetSettings();
    if(!cdb->connect())
        qDebug() << "Unable to Connect to DB";
    else
        qDebug() << "Connected to Database" ;

    load_final_modules();
    //connect(ad,SIGNAL(isAdmin(QString)),cdb,SLOT(get_user_password(QString)));
    //connect(ad,SIGNAL(userAuthenticated()),this,SLOT(load_final_modules()));
    connect(cdb,SIGNAL(user_passwd(QString)),ad,SLOT(authenticate(QString)));
}

void AppManager::load_final_modules()
{
    //    Load Interface, Log, Print
    interface = new Interface();
    qDebug() << "interface created";
    fg_rollLe = 1;
    interface->rollnoLe->setFocus();
    cprinter = new CPrinter();
    cprinter->updateList(settings->getPrintPositions(0),0);

    bg = new QButtonGroup;

    bg->addButton(interface->bCb,0);
    bg->addButton(interface->cCb,1);
    bg->addButton(interface->tcCb,2);
    bg->setExclusive(false);
    this->type = "B";


    // Interface
    connect(interface->btable,SIGNAL(cellPressed(int,int)),this,SLOT(btableclicked(int,int)));
    connect(interface->bg,SIGNAL(buttonClicked(int)),this,SLOT(printc(int)));
    connect(interface->printButton,SIGNAL(pressed()),this,SLOT(print()));
    connect(interface->stname,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->rollnoLe,SIGNAL(returnPressed()),this,SLOT(onRollEntry()));
    connect(interface->rollnoLe,SIGNAL(rollfocus()),this,SLOT(rollfocus()));
    connect(interface->pdf,SIGNAL(pressed()),this,SLOT(topdf()));
    connect(interface->left,SIGNAL(pressed()),this,SLOT(toLeft()));
    connect(interface->right,SIGNAL(pressed()),this,SLOT(toRight()));
    connect(interface->settings,SIGNAL(pressed()),settings,SLOT(createSettingsInterface()));
    connect(interface->log,SIGNAL(pressed()),log,SLOT(createLogManager()));
    // Settings
    //connect(,SIGNAL(update(int)),this,SLOT(updatevalues(int)));
    // Native
    connect(bg,SIGNAL(buttonClicked(int)),this,SLOT(updatetype(int)));
    //connect(interface->print,SIGNAL(pressed()),this,SLOT(print()));
    //

    interface->showMaximized();
}

void AppManager::onRollEntry()
{
    // Extract Roll Number
    roll =  interface->rollnoLe->text().toUpper();

    if(roll.isEmpty())
    {

        return;
    }

    student = new Student;

    student->ctype[0] = ctype[0];
    student->ctype[1] = ctype[1];
    student->ctype[2] = ctype[2];
    student->roll = roll;

    // Query based on Modes (Bonafide, Conduct, TC)
    if(cdb->fetch(ctype[2],student))
    {
        // Make Entry into the table
        interface->btable->add_roll(student->roll,type);

        // TODO : Acedemic Year
        interface->fyear->setPlainText("2009");
        interface->tyear->setPlainText("2010");

        // set the previews
        interface->set_ct(student);

        // Append to the print List
        this->st_plist.append(student);

        // Clear the Roll Box
        interface->rollnoLe->clear();
        interface->rollnoLe->setFocus();
    }
    else
        delete student;
}

void AppManager::manualProcess()
{
    // Extract Roll Number
    roll =  interface->rollnoLe->text().toUpper();

    if(roll.isEmpty())
    {

        return;
    }

    student = new Student;
    student->ctype[0] = ctype[0];
    student->ctype[1] = ctype[1];
    student->ctype[2] = ctype[2];
    student->roll = roll;
    interface->btable->add_roll(student->roll,type);

    // Append to the print List
    this->st_plist.append(student);

    // Clear the Roll Box
    interface->rollnoLe->clear();
    interface->rollnoLe->setFocus();
}

void AppManager::previewLastRoll()
{
    // To get a preview the last Roll Number printed
}



void AppManager::itemchanged(int id, const QString &data)
{

    // Null Student list
    if (!st_ptr)
        return;

    switch(id)
    {
    case 0: st_plist.at(st_ptr)->name = data;
        break;
    case 1: st_plist.at(st_ptr)->co = data;
        break;
    case 2: st_plist.at(st_ptr)->father_name = data;
        break;
    case 3: st_plist.at(st_ptr)->cdetails = data;
        qDebug() << st_plist.at(st_ptr);
        break;
    case 4: st_plist.at(st_ptr)->purpose = data;
        break;
    case 5: // cdate
        break;
    case 6: st_plist.at(st_ptr)->std_ex->dob = data;
        break;
    case 7: st_plist.at(st_ptr)->std_ex->doa = data;
        break;
    case 8: st_plist.at(st_ptr)->std_ex->course = data;
        break;
    case 9: st_plist.at(st_ptr)->std_ex->branch = data;
        break;
    case 10 : st_plist.at(st_ptr)->std_ex->dol = data;
        break;
    case 11: st_plist.at(st_ptr)->std_ex->qualified = data;
        break;
    case 12: st_plist.at(st_ptr)->std_ex->dues = data;
        break;
    case 13: st_plist.at(st_ptr)->std_ex->disp_m = data;
        break;
    case 14: st_plist.at(st_ptr)->std_ex->conduct = data;
        break;
    case 15: st_plist.at(st_ptr)->std_ex->community = data;
    }
}

// bulktable cell clicked
void AppManager::btableclicked(int row,int col)
{
    //interface->vpic = 0;

    student = st_plist.at(row);

    interface->set_ct(student);
    fg_rollLe = 0;
    interface->btable->st_ptr = st_ptr = row;
    interface->cframe->setStyleSheet(BTABLE_FRAME_COLOR);

    for(int i = 0;i < 3; ++i)
    {
        if(student->ctype[i])
            bg->button(i)->setChecked(true);
        else
            bg->button(i)->setChecked(false);
    }
}


void AppManager::updatetype(int i)
{
    type = "";

    if(!fg_rollLe) // Table update
    {
        if(bg->button(i)->isChecked())
            student->ctype[i] = 1;
        else
            student->ctype[i] = 0;

        for(int i = 0;i < 3; ++i)
        {
            if(student->ctype[i])
            {
                switch(i)
                {
                case 0 : type += "B ";
                    break;
                case 1 : type += "C ";
                    break;
                case 2 : type += "T";
                }
            }
        }

        interface->btable->item(st_ptr,1)->setText(type);

        // Update Student Object
        if(student->ctype[2]  and  !student->stx_filled)
        {
            cdb->fetch(student->ctype[2],student);
            interface->set_ct(student);
        }
    }
    else
    {
        if(bg->button(i)->isChecked())
            ctype[i] = 1;
        else
            ctype[i] = 0;

        for(int i = 0;i < 3; ++i)
        {
            if(ctype[i])
            {
                switch(i)
                {
                case 0 : type += "B ";
                    break;
                case 1 : type += "C ";
                    break;
                case 2 : type += "T";
                }
            }
        }

    }


}

void AppManager::removeSt(int i)
{
    this->st_plist.removeAt(i);
}

void AppManager::rollfocus()
{

    interface->rollnoLe->setFocus();
    fg_rollLe = 1;
    interface->cframe->setStyleSheet(FRAME_COLOR);
    interface->btable->clearSelection();
    for(int i = 0;i < 3;++i)
    {
        if(ctype[i])
            bg->button(i)->setChecked(true);
        else
            bg->button(i)->setChecked(false);
    }


}

// Right & Left Navigation works only when
// the user clicks an entry in the btable

void AppManager::toRight()
{
    // Make Sure btable is selected
    if(fg_rollLe)
        return;
    student->vpic = (student->vpic + 1) % 3;
    for(int j=0; j<3;++j)
    {
        if(student->ctype[student->vpic])
            break;
        student->vpic = (student->vpic + 1) % 3;
    }

    interface->show_ct(student->ctype[2],student->vpic);
}

void AppManager::toLeft()
{
    // Make Sure btable is selected
    if(fg_rollLe)
        return;

    interface->pic[student->vpic]->setOpacity(0.0);
    student->vpic = (student->vpic + 5) % 3;


    for(int j=0; j<3;++j)
    {
        if(student->ctype[student->vpic])
            break;
        student->vpic = (student->vpic + 5) % 3;
    }

    interface->show_ct(student->ctype[2],student->vpic);

}

void AppManager::print()
{
    // Get Print Order Array from Settings

    // Show the Print Dialogue
    interface->printDialog();

}


void AppManager::printc(int i)
{
    int over;
    switch(i)
    {
    case 0 : // Printing Bonafide
        cprinter->printB(st_plist,0,QStringList());
        break;
    case 1 : // Prining Conduct
        cprinter->printC(st_plist);
        break;
    case 2: // Printing TC
        cprinter->printTC(st_plist);
        over = 1;
    }

    if(over)
    {
        interface->btable->clear();

        for(int j=0; j<= st_plist.count();++i)
        {
            delete st_plist.at(i);
        }

        st_plist.clear();
    }
}

void AppManager::updatevalues(int i)
{
    switch(i)
    {

    }

}

void AppManager::topdf()
{
    qDebug() << "to pdf";
}

AppManager::~AppManager()
{

}
