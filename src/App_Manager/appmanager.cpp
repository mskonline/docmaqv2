/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 M.Sai Kumar <msk.mymails@gmail.com>

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

#include "appmanager.h"
#include "thread.h"
#include "../Database_Manager/cdatabase.h"
#include "../interface/interface.h"
#include "../Print_Manager/cprinter.h"
#include "../Authentication_Manager/authendialog.h"
#include "../Settings_Manager/settings.h"
#include "../Settings_Manager/getsettings.h"
#include "../Log_Manager/log.h"

/*
 * Constructor
 */
AppManager::AppManager()
{
    // Splash Screen
    splash = new QSplashScreen;
    splash->setPixmap(QPixmap("./images/stpic.png"));
    splash->show();

    Qt::Alignment bottomleft = Qt::AlignLeft | Qt::AlignBottom;
    splash->showMessage("Initializing...",bottomleft, Qt::white);

    // Initialize few essential flags
    st_ptr = 0;
    et = 0;
    fg_rollLe = 1;

    c_type[0] = 1;
    c_type[1] = 0;
    vpic = 0;
    c_mode = p_type =  0;
    c_text = t_text = false;

    admn = 0;
    c_count.append(0);
    c_count.append(0);

    this->rtype = this->btype =  "B";
    st_plist = new QList <Student *>;

    Delay::sleep(1);

    // load the initial modules
    load_init_modules();
}

/* load_init_modules()
 * Called from AppManager Constructor
 * loads initial modules
 */
void AppManager::load_init_modules()
{
    // Splash Screen
    Qt::Alignment bottomleft = Qt::AlignLeft | Qt::AlignBottom;
    splash->showMessage("Loading Settings...",bottomleft, Qt::white);

    // Load DB, Auth, Settings
    settings=new GetSettings();
    settings->getMode(d_mode);
    Delay::sleep(1);

    splash->showMessage("Connecting to Database...",bottomleft, Qt::white);
    cdb = new CDatabase();

    // Check for Database Mode
    if(d_mode)
    {
        settings->getDatabaseDetails(dblist);
        cdb->connect(dblist);
    }

    Delay::sleep(1);
    splash->close();

    // Bring up the Authentication Dialog
    ad=new AuthenDialog(d_mode,cdb->is_connected,cdb->dbstatus);
    ad->show();

    if(!cdb->is_connected)
    {
        d_mode = false;
        settings->setMode(d_mode);
    }

    // Connections in Authen Manager
    connect(ad,SIGNAL(checkuser(QString)),cdb,SLOT(get_user_password(QString)));
    connect(ad,SIGNAL(userAuthenticated(int)),this,SLOT(load_final_modules(int)));
    connect(cdb,SIGNAL(user_passwd(QString)),ad,SLOT(authenticate(QString)));

    delete splash;
}

/* load_final_modules()
 * Called by AuthenDialog only when the
 * Username & password are valid
 */
void AppManager::load_final_modules(int admn)
{
    this->admn = admn;

    switch(admn)
    {
    case 0 : cdb->username = "Staff";
        cdb->user_id = "Staff";
        break;
    case 1 : cdb->username = "Admin";
        cdb->user_id = "Admin";
    }

    lg = new d_log;
    lg->startSession(cdb->user_id,cdb->username);

    // Interface Initialization from settings , (csno,cdate,acyears)
    settings->getSno(c_sno);
    r_sno[0] = c_sno[0];
    r_sno[1] = c_sno[1];

    settings->getDateAcademicYear(dlist);

    //Load Interface, Log, Print
    interface = new Interface();
    interface->set_items(dlist);
    interface->rollnoLe->setFocus();

    // Set Status bar
    interface->setSBText(cdb->username,d_mode);
    interface->rbb->setChecked(true);

    // Load Print Manager
    cprinter = new CPrinter(c_sno,dlist);
    cprinter->setPrinter();
    cprinter->st_list = st_plist;

    // Settings
    settings->getPrinter(cprinter->p);
    settings->getPrintPositions(cprinter->blist,0);
    settings->getPrintPositions(cprinter->clist,1);

    bg_ctype = new QButtonGroup;
    bg_ctype->addButton(interface->bCb,0);
    bg_ctype->addButton(interface->cCb,1);
    bg_ctype->setExclusive(false);

    bg_ptype = new QButtonGroup;
    bg_ptype->addButton(interface->rbb,0);
    bg_ptype->addButton(interface->rbc,1);
    bg_ptype->setExclusive(true);

    // Interface
    connect(interface->btable,SIGNAL(cellPressed(int,int)),this,SLOT(btableclicked(int,int)));
    connect(interface->btable,SIGNAL(removeSt(int)),this,SLOT(removeSt(int)));
    connect(interface->btable,SIGNAL(cellChanged(int,int)),this,SLOT(revertDetails(int)));
    connect(interface->btable,SIGNAL(b_clearRow()),this,SLOT(b_clearRow()));

    connect(interface->stname,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->fname,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->co,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->cdetails,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->purpose,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->acyear,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));

    connect(interface->rollnoLe,SIGNAL(returnPressed()),this,SLOT(onRollEntry()));
    connect(interface->rollnoLe,SIGNAL(rollfocus()),this,SLOT(rollfocus()));

    connect(interface->printButton,SIGNAL(released()),this,SLOT(print()));
    connect(interface->tcb,SIGNAL(released()),this,SLOT(TC_mode()));
    connect(interface->settingsb,SIGNAL(released()),this,SLOT(createSettingsUI()));
    connect(interface->logb,SIGNAL(released()),this,SLOT(createLogUI()));
    connect(interface->pdfb,SIGNAL(released()),this,SLOT(pdfprint()));

    connect(interface->larrow,SIGNAL(pressed()),this,SLOT(toLeft()));
    connect(interface->rarrow,SIGNAL(pressed()),this,SLOT(toRight()));
    connect(interface->uarrow,SIGNAL(pressed()),this,SLOT(toup()));
    connect(interface->darrow,SIGNAL(pressed()),this,SLOT(todown()));
    connect(interface->session,SIGNAL(pressed()),this,SLOT(createSessionPage()));

    connect(interface->prints->nprint,SIGNAL(released()),cprinter,SLOT(printc()));
    connect(interface,SIGNAL(quit()),this,SLOT(quit()));

    // Shortcuts
    connect(interface->Bsc,SIGNAL(activated()),this,SLOT(b_print()));
    connect(interface->Csc,SIGNAL(activated()),this,SLOT(c_print()));

    connect(cdb,SIGNAL(report(int,QString)),interface,SLOT(report(int,QString)));

    connect(cprinter,SIGNAL(pcomplete(int)),this,SLOT(pcomplete(int)));
    connect(cprinter,SIGNAL(tc_pcomplete()),this,SLOT(tc_pcomplete()));
    connect(bg_ctype,SIGNAL(buttonClicked(int)),this,SLOT(updatetype(int)));
    connect(bg_ptype,SIGNAL(buttonClicked(int)),this,SLOT(update_print_type(int)));

    // Remove Authen Manager Connections
    disconnect(ad,SIGNAL(checkuser(QString)),cdb,SLOT(get_user_password(QString)));
    disconnect(ad,SIGNAL(userAuthenticated(int)),this,SLOT(load_final_modules(int)));
    disconnect(cdb,SIGNAL(user_passwd(QString)),ad,SLOT(authenticate(QString)));

    interface->showMaximized();
}

/* onRollEntry()
 * Called : when the user presses "Enter" Key in the rollnole
 * The Roll Number in the LineEdit is extracted
 * Performs : Depending on the Mode, Student Data is Collected
 */
void AppManager::onRollEntry()
{
    // Check if it is in TC Mode
    if(c_mode)
    {
        tc_student->roll = interface->troll->text().toUpper();
        tc_student->st_filled = 0;
        tc_student->c_sno[0] = c_sno[2];

        if(!cdb->fetch_ex(tc_student))
        {
            tc_student->st_filled = 0;
            return;
        }
        cdb->format_tc(tc_student);
        interface->set_tc(tc_student);

        interface->rollDisp->setText("<font color='white'>" + tc_student->roll + "</font>");
        interface->setWindowTitle("DocmaQ : " + tc_student->roll);
        return;
    }

    // Extract Roll Number
    roll =  interface->rollnoLe->text().toUpper();

    // Check for Empty Roll
    if(roll.isEmpty())
    {
        if(interface->btable->rc == 0)
        {
            // Report empty entry
            interface->report(ERROR,"Roll Box is Empty. Please Fill it with correct Roll Number.");
            return;
        }
        else
        {
            this->print();
            return;
        }
    }

    // Check if roll already exits.
    if(!interface->btable->findItems(roll,Qt::MatchCaseSensitive).isEmpty())
    {
        interface->report(ERROR,"Roll " + roll + " already exits.");
        return;
    }

    // Student Object
    student = new Student;
    student->c_type[0] = c_type[0];
    student->c_type[1] = c_type[1];
    student->roll = roll;

    // Check for Mode, Manual/DB
    if(d_mode)
    {
        // Database Mode
        if(!cdb->fetch(student))
        {
            delete student;
            rollfocus();
            return;
        }

        cdb->format_course(student);
    }

    // Set List Pointer
    st_ptr = interface->btable->rc;

    // Make Entry into the table
    interface->btable->add_roll(student->roll,rtype);

    // Clear the Roll Box
    interface->rollnoLe->clear();
    interface->rollnoLe->setFocus();

    // Update the count of each certificates
    c_count[0] = c_count[0] + student->c_type[0];
    interface->blcd->display(c_count[0]);
    c_count[1] = c_count[1] + student->c_type[1];
    interface->clcd->display(c_count[1]);

    // Assign Serial No. to this entry (bonafide or conduct only)
    for(int i = 0; i < 2 ; ++i)
    {
        if(student->c_type[i])
        {
            student->c_sno[i] = r_sno[i];
            r_sno[i] = r_sno[i] % 9999 + 1;
        }
    }

    // Append to the print List
    this->st_plist->append(student);

    // Set the interface
    interface->set_ct(student);  

    // Set roll in Roll Display
    interface->rollDisp->setText("<font color='white'>" + student->roll + "</font>");

    // Set the Window Title
    interface->setWindowTitle("DocmaQ : " + student->roll);
}

/* itemchanged(int,const QString)
 * Called : by each QGraphicTextItem upon change
 * in its Contents.
 * Performs : btable enabling/disabling
 */
void AppManager::itemchanged(int id, const QString &data)
{
    // Disable btable when a item is being edited
    if(id < 0 )
    {
        interface->setenabled(false);
        et += id;
        return;
    }

    switch(id)
    {
    case 1: st_plist->at(st_ptr)->name = data;
        break;
    case 2: st_plist->at(st_ptr)->co = data;
        break;
    case 3: st_plist->at(st_ptr)->father_name = data;
        break;
    case 4: st_plist->at(st_ptr)->cdetails = data;
        break;
    case 5: st_plist->at(st_ptr)->purpose = data;
        break;
    case 6 : st_plist->at(st_ptr)->acyear = data;
    }

    et += id;

    if(et == 0)
        interface->setenabled(true);

}

/* revertDetails(int)
 * Called : when User Selects 'Revert Details' option in
 * the context menu of a roll entry
 * Performs : details retrival for changed Roll Number
 */
void AppManager::revertDetails(int r)
{
    if(interface->rollnoLe->hasFocus())
        return;

    if(c_text or t_text)
    {
        c_text = t_text = false;
        return;
    }

    // Check if its in DB Mode
    if(d_mode)
    {
        st_plist->at(r)->roll = interface->btable->item(r,0)->text().toUpper();
        st_plist->at(r)->st_filled = 0;
        // Database Mode
        if(!cdb->fetch(st_plist->at(r)))
        {
            QMessageBox msgBox;
            msgBox.setParent(interface);
            msgBox.setWindowFlags(Qt::Window);
            msgBox.setInformativeText("Do you wish to Continue editing this entry or want to Remove it ?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox.setDefaultButton(QMessageBox::Yes);
            int ret = msgBox.exec();

            switch(ret)
            {
            case QMessageBox::Yes : interface->btable->openPersistentEditor(interface->btable->item(r,0));
                return;
                break;
            case QMessageBox::No : removeSt(r);
            }
        }

        cdb->format_course(st_plist->at(r));
        interface->btable->closePersistentEditor(interface->btable->item(r,0));
        interface->set_ct(st_plist->at(r));
    }
    else
    {
        // Manual Mode
        st_plist->at(r)->roll = interface->btable->item(r,0)->text();
        interface->btable->closePersistentEditor(interface->btable->item(r,0));
        interface->set_ct(st_plist->at(r));
    }
}


/* btableclicked(int,int)
 * Called : when a btable cell is clicked
 * Performs : Sets the preview corresponding to the
 * Roll Number Clicked
 */
void AppManager::btableclicked(int row,int col)
{
    student = st_plist->at(row);

    interface->set_ct(student);
    fg_rollLe = 0;
    interface->btable->st_ptr = st_ptr = row;
    interface->cframe->setStyleSheet(BTABLE_FRAME_COLOR);

    for(int i = 0;i < 2; ++i)
    {
        if(student->c_type[i])
            bg_ctype->button(i)->setChecked(true);
        else
            bg_ctype->button(i)->setChecked(false);
    }

    // Set Window Title
    interface->setWindowTitle("DocmaQ : " + student->roll);

    interface->rollDisp->setText("<font color='white'>" + student->roll +"</font>");
}

/* updatetype(int)
 * Called : when user changes the Certificate type
 * through checkboxes
 * Performs : Updates the Certificate type for
 * the corresponding roll
 */
void AppManager::updatetype(int i)
{
    if(!fg_rollLe) // Table update
    {
        btype = "";
        if(bg_ctype->button(i)->isChecked())
        {
            student->c_type[i] = 1;
            update_counts(i,1);
        }
        else
        {
            student->c_type[i] = 0;
            // Atleast one type needs to be checked !
            if(student->c_type[0] + student->c_type[1] == 0)
            {
                student->c_type[i] = 1;
                bg_ctype->button(i)->setChecked(true);
                //report error
                interface->report(INFO,"Atleast One Certificate needs to be Selected.");
            }
            else
                update_counts(i,-1);
        }

        for(int j = 0;j < 2; ++j)
        {
            if(student->c_type[j])
            {
                switch(j)
                {
                case 0 : btype += "B ";
                    break;
                case 1 : btype += "C ";
                }
            }
        }

        t_text = true;
        interface->btable->item(st_ptr,1)->setText(btype);
        interface->set_ct(student);
    }
    else
    {
        rtype = "";
        if(bg_ctype->button(i)->isChecked())
            c_type[i] = 1;
        else
        {
            c_type[i] = 0;

            // Atleast one type needs to be checked !
            if(c_type[0] + c_type[1] == 0)
            {
                //report error
                c_type[i] = 1;
                interface->report(INFO,"Atleast One Certificate needs to be Selected.");
                bg_ctype->button(i)->setChecked(true);
            }
        }

        for(int j = 0; j < 2 ; ++j)
        {
            if(c_type[j])
            {
                switch(j)
                {
                case 0 : rtype += "B ";
                    break;
                case 1 : rtype += "C ";
                }
            }
        }

        for(int i =0; i < 2;++i)
        {
            if(c_type[i])
            {
                interface->show_ct(i);
                vpic = i;
                break;
            }
        }
    }
}

/* removeSt(int)
 * Called : by btable when an roll number is deleted
 * by pressing "delete" key
 * Performs : Deletes the Roll entry in btable
 */
void AppManager::removeSt(int i)
{
    // i = -1 implies clearing all entries
    if(i == -1)
    {
        // Set all Counts to Zero
        c_count[0] = 0;
        c_count[1] = 0;

        // Set the Count Displays
        interface->blcd->display(c_count[0]);
        interface->clcd->display(c_count[1]);

        // Delete all entries
        for(int j = 0; j < st_plist->count() ; ++j)
            delete st_plist->at(j);

        st_plist->clear();

        r_sno[0] = c_sno[0];
        r_sno[1] = c_sno[1];

        rollfocus();

        // Window Title
        interface->setWindowTitle("DocmaQ");

        interface->rollDisp->setText("");
        return;
    }

    // Update Counts
    c_count[0] = c_count[0] - st_plist->at(i)->c_type[0];
    interface->blcd->display(c_count[0]);

    c_count[1] = c_count[1] - st_plist->at(i)->c_type[1];
    interface->clcd->display(c_count[1]);

    int c_type[2];
    c_type[0] = st_plist->at(i)->c_type[0];
    c_type[1] = st_plist->at(i)->c_type[1];

    // Release the memory & clean up the entry
    delete this->st_plist->at(i);
    this->st_plist->removeAt(i);

    // Update Serial No.
    for(int j = 0 ; j < 2; ++j)
    {
        if(c_type[j])
            this->recalculate_csno(j);
    }

    // Set Window Title and Roll Display
    if(interface->btable->rowCount() == 0)
    {
         interface->setWindowTitle("DocmaQ");
         interface->rollDisp->setText("");
    }

    rollfocus();
}

/*
 * Check for whether an Roll Entry is
 * Clicked or not
 */
void AppManager::b_clearRow()
{
    if(!fg_rollLe)
        interface->btable->clearRow();
}

/* rollfocus()
 * Called : when the user doubleclicks the rollnolLe
 * Performs :
 */
void AppManager::rollfocus()
{
    if(c_mode)
        return;

    //rollnoLe now has the focus
    fg_rollLe = 1;

    interface->cframe->setStyleSheet(FRAME_COLOR);
    interface->btable->clearSelection();

    int i;
    for(i = 0;i < 2;++i)
        bg_ctype->button(i)->setChecked(c_type[i]);

    for(i = 0;i < 2;++i)
    {
        if(bg_ctype->button(i)->isChecked())
            break;
    }

    interface->btable->st_ptr = st_ptr = -1;
    vpic = i;
    interface->resetView(i);
}

/* Right & Left Navigation works only when
 * the user clicks on an entry in the btable
 */
void AppManager::toRight()
{
    if(fg_rollLe)
    {
        if(c_type[!vpic])
        {
            interface->show_ct(!vpic);
            vpic = !vpic;
        }
        return;
    }

    if(student->c_type[!student->vpic])
    {
        interface->pic[student->vpic]->setOpacity(0.0);
        student->vpic = !student->vpic;
        interface->sno->setPlainText(temp.sprintf("%04d",st_plist->at(st_ptr)->c_sno[student->vpic]));
        interface->show_ct(student->vpic);
    }
}

void AppManager::toLeft()
{
    if(fg_rollLe)
    {
        if(c_type[!vpic])
        {
            interface->show_ct(!vpic);
            vpic = !vpic;
        }
        return;
    }

    if(student->c_type[!student->vpic])
    {
        interface->pic[student->vpic]->setOpacity(0.0);
        student->vpic = !student->vpic;
        interface->sno->setPlainText(temp.sprintf("%04d",st_plist->at(st_ptr)->c_sno[student->vpic]));
        interface->show_ct(student->vpic);
    }
}

void AppManager::toup()
{
    if(fg_rollLe)
        return;

    st_ptr = (st_ptr + interface->btable->rc - 1) % interface->btable->rc;

    btableclicked(st_ptr, -1);
    interface->btable->selectRow(st_ptr);
}

void AppManager::todown()
{
    if(fg_rollLe)
        return;

    st_ptr = (st_ptr + 1) % interface->btable->rc;

    btableclicked(st_ptr, -1);
    interface->btable->selectRow(st_ptr);
}

void AppManager::update_print_type(int r)
{
    switch(r)
    {
    case 0 : // All Bonafides
        interface->prints->hlabel->setText("<font color='white'>Bonafide Certificate Printing</font>");
        interface->prints->tlabel->setText("Bonafide");
        break;
    case 1: // All Conduct
        interface->prints->hlabel->setText("<font color='white'>Conduct Certificate Printing</font>");
        interface->prints->tlabel->setText("Conduct");
    }

    cprinter->p_type = p_type = r;
}

/* void b_print()
 * Called when user presses F1 key
 */
void AppManager::b_print()
{
    // printing is locked, an item is being changed
    if(!interface->btable->isEnabled())
        return;

    if(!c_count[0])
        return;

    update_print_type(0);
    init_print(0);
}

/* void c_print()
 * Called when user presses F2 key
 */
void AppManager::c_print()
{
    // printing is locked, an item is being changed
    if(!interface->btable->isEnabled())
        return;

    if(!c_count[1])
        return;

    update_print_type(1);
    init_print(1);
}

/* print()
 * Called when user presses the print button
 */
void AppManager::print()
{
    // Check for TC Mode
    if(c_mode)
    {
        if(tc_student->st_filled)
            init_print(2);
        return;
    }

    // Check for zero count
    if(!c_count[p_type])
        return;

    init_print(p_type);

    interface->setWindowTitle("DocmaQ");
    interface->rollDisp->setText("");
}

/* init_print()
 * Called by print(), b_print(), c_print()
 * Performs mandatory initializations
 */
void AppManager::init_print(int lp_type)
{
    if(c_mode)
    {
        if(admn == 1)
        {
            cprinter->processTC();

            cprinter->admin = 0;
            cprinter->count = 2;
            TC_mode();
            relogin();
            return;
        }

        if(tc_student->std_ex->is)
        {
            interface->dissue= new DIssueDialog(interface,tc_student);
            interface->dissue->move(interface->width()/2 - 225 , interface->height()/2 - 259);
            interface->dissue->show();
            connect(interface->dissue,SIGNAL(print()),this,SLOT(TC_dissue()));
            return;
        }

        // Print Dialog Box
        QMessageBox msgBox;
        msgBox.setParent(interface);
        msgBox.setWindowFlags(Qt::Window);
        msgBox.setText("This is the First Issue of Transfer Certificate for " + tc_student->roll);
        msgBox.setInformativeText("Do you want to print it ?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch(ret)
        {
        case QMessageBox::Yes :
            if(cdb->log_tc(tc_student))
                return;

            cprinter->processTC();
            break;
        case QMessageBox::No : return;
        }
    }
    else
    {
        interface->prints->clabel->setText(QString().sprintf("%d",c_count.at(lp_type)));
        // Show the Print Dialogue
        interface->prints->move(interface->width()/2 - 230 , interface->height()/2 - 94);
        interface->prints->show();
        cprinter->p_type = lp_type;
    }
}

/* pdfprint()
 * Called wneh user selects 'Print PDF' button in left panel'
 * Performs PDF printing
 */
void AppManager::pdfprint()
{
    if(c_mode)
        return;

    interface->pdfb->setDown(false);
    if(interface->btable->rowCount() == 0)
        return;

    cprinter->pdfprint(c_count[0],c_count[1]);

    interface->setWindowTitle("DocmaQ");
    interface->rollDisp->setText("");
}

/* TC_dissue
 * Called by Issue Duplicate Button of 'Issue Duplicate' Widget
 * Performs Duplicate Issue Printing
 */
void AppManager::TC_dissue()
{
    // replace dissue date to cdate,issued_by
    QString dt = QDate::currentDate().toString("yyyy-MM-dd");
    tc_student->std_ex->dissue->replace(3,dt);
    tc_student->std_ex->dissue->replace(4,cdb->username);

    if(!cdb->update_dissue(tc_student))
        cprinter->processTC();
}

void AppManager::tc_type(int s)
{
    if(s == 0)
    {
        interface->tc_type->setPlainText("DUPLICATE");
        cprinter->t = 1;
    }
    else
    {
        interface->tc_type->setPlainText("ORIGINAL");
        cprinter->t = 0;
    }
}

void AppManager::sno_changed(int t)
{
    tc_student->c_sno[0] = t;
    interface->tc_sno->setPlainText(temp.sprintf("%05d",tc_student->c_sno[0]));
}

/* pcomplete(int)
 * Called by CPrinter::printB(), CPrinter::printC()
 * Performs btable and st_plist cleanup
 */
void AppManager::pcomplete(int ctype)
{
    interface->prints->hide();
    rollfocus();

    for(int j = 0; j < st_plist->count(); ++j)
    {
        if(st_plist->at(j)->c_type[ctype])
        {
            st_plist->at(j)->c_type[ctype] = 0;

            // write log
            lg->writeCertificateLog(st_plist->at(j)->roll,ctype);

            // Entry has no certificates
            if(st_plist->at(j)->c_type[!ctype]  == 0)
            {
                delete st_plist->at(j);
                st_plist->removeAt(j);
                interface->btable->removeRow(j);
                --j;
                --interface->btable->rc;
            }
            else // Update Entry
            {
                if(ctype)
                    temp = "B ";
                else
                    temp = " C";
                c_text = true;
                interface->btable->item(j,1)->setText(temp);
            }

            if(!interface->btable->rowCount())
            {
                interface->btable->rc = 0;
                break;
            }
        }
    }

    c_count[ctype] = 0;

    if(!ctype)
        interface->blcd->display(c_count[0]);
    else
        interface->clcd->display(c_count[1]);
}

void AppManager::tc_pcomplete()
{
    if(admn != 1)
    {
         c_sno[2] = c_sno[2] % 99999 + 1;
         lg->writeCertificateLog(tc_student->roll,2);
    }

    interface->tc_sno->setPlainText("");
    interface->tc_type->setPlainText("");
    interface->stname->setPlainText("");
    interface->fname->setPlainText("");
    interface->roll->setPlainText("");
    interface->admno->setPlainText("");

    for(int i = 0 ; i < 11 ;++i)
        interface->tcitems->at(i)->setPlainText("");

    tc_student->st_filled = 0;

    interface->troll->clear();
    interface->troll->setFocus();
}

/* updatevalues()
 * Called by Settings
 * Performs updations of details
 */
void AppManager::updatevalues(QList <int> &ulist)
{
    if(ulist.isEmpty())
        return;

    for(int i = 0; i < ulist.count(); ++i)
    {
        switch(ulist[i])
        {
            case 0 : // Mode
                settings->getMode(d_mode);
                if(d_mode)
                {
                    settings->getDatabaseDetails(dblist);
                    if(!cdb->reconnect(dblist))
                    {
                        interface->report(2,tr("An Error Occured while Connecting to the Database.") +
                                          "Please Check the Connection details.The Application will now run in <b>Manual Mode</b><br><br>" +
                                          tr("MySQL Reports : ") + cdb->dbstatus );
                        d_mode = false;
                        settings->setMode(d_mode);
                    }
                    else
                    {
                        if(cdb->username == "Staff")
                            relogin();
                    }
                }
                else
                {
                    if(c_mode)
                    {
                        d_mode = true;
                        TC_mode();
                        d_mode = false;
                    }
                }
                modechange();
                break;
            case 1 : // Serial No.
                settings->getSno(c_sno);
                if(!st_plist->isEmpty())
                {
                    recalculate_csno(0);
                    recalculate_csno(1);
                }

                if(c_mode)
                {
                    tc_student->c_sno[0] = c_sno[2];
                    interface->tc_sno->setPlainText(temp.sprintf("%05d",c_sno[2]));
                }
                break;
            case 2 : // Date , Academic years
                settings->getDateAcademicYear(dlist);

                if(c_mode)
                    break;

                interface->set_items(dlist);
                break;
            case 3 : // Printer
                settings->getPrinter(cprinter->p);
                cprinter->setPrinter();

                if(c_mode)
                {
                    cprinter->setTCprint(0);
                    cprinter->setTCprint(1);
                }
                break;
            case 4 : // Bonafide Print Positions
                settings->getPrintPositions(cprinter->blist,0);
                break;
            case 5 : // Conduct Print Positions
                settings->getPrintPositions(cprinter->clist,1);
                break;
            case 6: // TC Print Positions
                if(c_mode)
                    settings->getPrintPositions(cprinter->tlist,2);
         }
    }

    lg->openlog();
    rollfocus();
}

void AppManager::update_counts(int ctype, int c)
{
    c_count[ctype] = c_count[ctype] + c;

    if(ctype)
        interface->clcd->display(c_count.at(ctype));
    else
        interface->blcd->display(c_count.at(ctype));

    // Updating Serial Numbers of this type
    recalculate_csno(ctype) ;
}

void AppManager::recalculate_csno(int i)
{
    int j, s;
    s = c_sno[i];
    for(j = 0 ; j < st_plist->count(); ++j)
    {
        if(st_plist->at(j)->c_type[i])
        {
            st_plist->at(j)->c_sno[i] = s;
            s = s % 9999 + 1;
        }
    }

    this->r_sno[i] = s;
}

void AppManager::modechange()
{
    // Database Mode : mode = true
    // Manual Mode : mode = false
    if(d_mode)
        interface->mode->setText("Mode : " + tr("<b>") + "Database" + tr("<b>"));
    else
        interface->mode->setText("Mode : " + tr("<b>") + "Manual" + tr("<b>"));
}

void AppManager::TC_mode()
{
    // TC Modes requires application to be in DB Mode
    if(d_mode == 0)
    {
        interface->report(ERROR, "TC can be issued only in <b>Database Mode.</b>");
        interface->tcb->setDown(false);
        return;
    }

    c_mode = !c_mode;

    switch(c_mode)
    {
    case 0 : // Set to Normal Mode
        interface->wframe->setVisible(true);
        interface->set_TC_Items(0);
        interface->tcb->setText("Issue TC");
        interface->set_items(dlist);

        cprinter->setTCprint(false);
        delete tc_student->std_ex->dissue;
        delete tc_student->std_ex;
        delete tc_student;
        cprinter->tc_student = 0;
        interface->rollnoLe->setFocus();
        break;
    case 1: // Set to TC Mode
        interface->wframe->setVisible(false);
        interface->set_TC_Items(1);
        interface->tcb->setText("Close TC");
        interface->cdate->setPlainText(QDate::currentDate().toString("dd.MMM.yyyy"));

        // Check if Admin is logged in.
        if(admn == 1)
        {
            cprinter->admin = admn;
            interface->report(INFO,"As Admin, you can Modify Serial No. and Certificate Type in <b>One</b> Issue Only.");

            interface->typecb->setEnabled(true);
            interface->sersb->setEnabled(true);

            connect(interface->typecb,SIGNAL(stateChanged(int)),this,SLOT(tc_type(int)));
            connect(interface->sersb,SIGNAL(valueChanged(int)),this,SLOT(sno_changed(int)));
        }

        cprinter->setTCprint(true);
        settings->getPrintPositions(cprinter->tlist,2);
        tc_student = new Student;
        tc_student->std_ex = new Student_ex;
        tc_student->std_ex->dissue = new QStringList;
        cprinter->tc_student = tc_student;

        connect(interface->tprint,SIGNAL(released()),this,SLOT(print()));
        connect(interface->troll,SIGNAL(returnPressed()),this,SLOT(onRollEntry()));
    }

    interface->setWindowTitle("DocmaQ");
    interface->rollDisp->setText("");
}

void AppManager::createLogUI()
{
    lg->createlogmanager(interface);
    interface->logb->setDown(false);
}

void AppManager::createSettingsUI()
{
    lg->closelog();

    settings->createSettingsInterface(interface,admn);
    connect(settings->st,SIGNAL(sendSignal(QList<int>&)),this,SLOT(updatevalues(QList <int> &)));

    interface->settingsb->setDown(false);
}

void AppManager::createSessionPage()
{
    QStringList sinfo;   
    settings->getSessionInfo(sinfo);
    interface->showsessionpage(sinfo);
}

/* void quit()
 * Called upon interface Close Event
 * Checks for any entries left in btable for printing
 */
void AppManager::quit()
{
    // Check if any entries in btable
    if(!st_plist->isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setParent(interface);
        msgBox.setWindowFlags(Qt::Window);
        msgBox.setText("They are Roll Numbers waiting to be Printed      ");
        msgBox.setInformativeText("Do you want to <b>Quit</b> without Printing them ?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch(ret)
        {
        case QMessageBox::Yes : lg->endSession();
            qApp->quit();
            break;
        case QMessageBox::No :;
        }
    }
    else
    {
        lg->endSession();
        qApp->quit();
    }
}

void AppManager::relogin()
{
    interface->hide();
    ad=new AuthenDialog(d_mode,cdb->is_connected,cdb->dbstatus);
    ad->show();

    connect(ad,SIGNAL(checkuser(QString)),cdb,SLOT(get_user_password(QString)));
    connect(ad,SIGNAL(userAuthenticated(int)),this,SLOT(userchange(int)));
    connect(cdb,SIGNAL(user_passwd(QString)),ad,SLOT(authenticate(QString)));
    lg->endSession();
}

void AppManager::userchange(int admn)
{
    this->admn = admn;
    settings->getMode(d_mode);

    switch(admn)
    {
    case 0 : cdb->username = "Staff";
        cdb->user_id = "Staff";
        break;
    case 1 : cdb->username = "Admin";
        cdb->user_id = "Admin";
    }

    interface->setSBText(cdb->username,d_mode);
    interface->showMaximized();
    lg->startSession(cdb->user_id,cdb->username);
    rollfocus();
}

AppManager::~AppManager()
{
    delete cdb;
    delete settings;
}
