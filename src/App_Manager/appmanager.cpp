#include "appmanager.h"
#include "../Database_Manager/cdatabase.h"
#include "../interface/interface.h"
#include "../Print_Manager/cprinter.h"
#include "../Authentication_Manager/authendialog.h"
#include "../Settings_Manager/settings.h"
#include "../Settings_Manager/getsettings.h"
#include "../Log_Manager/log.h"

#include <QDebug>

AppManager::AppManager()
{
    /*splash = new QSplashScreen;
    splash->setPixmap(QPixmap("./stpic.png"));
    splash->show();

    Qt::Alignment bottomleft = Qt::AlignLeft | Qt::AlignBottom;
    splash->showMessage(QObject::tr("Initializing..."),bottomleft, Qt::white);*/

    // Initialize few essential flags
    st_ptr = 0;
    et =p_lock = 0;
    fg_rollLe = 1;

    c_type[0] = 1;
    c_type[1] = 0;

    c_mode = p_type = 0;

    c_sno.append(0);
    c_sno.append(0);
    c_count.append(0);
    c_count.append(0);

    this->rtype = this->btype =  "B";
    st_plist = new QList <Student *>;

    // load the initial modules
    load_init_modules();
}

/* load_init_modules()
 * Called from AppManager Constructor
 * loads initial modules
 */
void AppManager::load_init_modules()
{
    // Load DB, Auth, Settings
    cdb = new CDatabase();
    cdb->connect();

    ad=new AuthenDialog();
    ad->show();

    log= new logs(new QTextEdit);
    settings=new GetSettings();


    connect(ad,SIGNAL(isAdmin(QString)),cdb,SLOT(get_user_password(QString)));
    connect(ad,SIGNAL(userAuthenticated()),this,SLOT(load_final_modules()));
    connect(cdb,SIGNAL(user_passwd(QString)),ad,SLOT(authenticate(QString)));
}

/* load_final_modules()
 * Called by AuthenDialog only when the
 * Username & password are valid
 */
void AppManager::load_final_modules()
{
    //Load Interface, Log, Print
    interface = new Interface();
    interface->rollnoLe->setFocus();

    if(cdb->is_connected)
        d_mode = 1;
    else
        d_mode = 0;

    // Set Status bar
    interface->setStatusBar(cdb->username,d_mode);
    interface->rbb->setChecked(true);

    // Interface Initialization from settings , csno

    cprinter = new CPrinter(c_sno,dlist);
    //cprinter->updateList(settings->getPrintPositions(0),0);

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

    connect(interface->stname,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->fname,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->co,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->cdetails,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    connect(interface->purpose,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));

    connect(interface->rollnoLe,SIGNAL(returnPressed()),this,SLOT(onRollEntry()));
    connect(interface->rollnoLe,SIGNAL(rollfocus()),this,SLOT(rollfocus()));

    connect(interface->printButton,SIGNAL(pressed()),this,SLOT(print()));
    connect(interface->tcb,SIGNAL(pressed()),this,SLOT(TC_mode()));
    connect(interface->larrow,SIGNAL(pressed()),this,SLOT(toLeft()));
    connect(interface->rarrow,SIGNAL(pressed()),this,SLOT(toRight()));
    connect(interface->uarrow,SIGNAL(pressed()),this,SLOT(toup()));
    connect(interface->darrow,SIGNAL(pressed()),this,SLOT(todown()));
    connect(interface->prints->nbutton,SIGNAL(pressed()),cprinter,SLOT(printc()));

    connect(interface,SIGNAL(quit()),this,SLOT(quit()));
    connect(cdb,SIGNAL(report(int,QString)),interface,SLOT(report(int,QString)));

    connect(interface->settingsb,SIGNAL(pressed()),settings,SLOT(createSettingsInterface()));
    connect(interface->logb,SIGNAL(pressed()),log,SLOT(createLogManager()));
    connect(interface->prints->nbutton,SIGNAL(pressed()),cprinter,SLOT(printc()));

    connect(interface->Bsc,SIGNAL(activated()),this,SLOT(b_print()));
    connect(interface->Csc,SIGNAL(activated()),this,SLOT(c_print()));
    //connect(interface->help,SIGNAL(activated()),this,SLOT());

    // Settings
    //connect(,SIGNAL(update(int)),this,SLOT(updatevalues(int)));
    // Native
    connect(bg_ctype,SIGNAL(buttonClicked(int)),this,SLOT(updatetype(int)));
    connect(bg_ptype,SIGNAL(buttonClicked(int)),this,SLOT(update_print_type(int)));
}

/* onRollEntry()
 * Called when the user presses "Enter" Key in the rollnole
 * The Roll Number in the LineEdit is extracted
 * Depending on the Mode, Student Data is Collected
 */
void AppManager::onRollEntry()
{
    // Extract Roll Number
    roll =  interface->rollnoLe->text().toUpper();

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
            this->init_print(p_type);
            return;
        }
    }

    // Check if it is in TC Mode
    if(c_mode)
    {
        tc_student = new Student;
        tc_student->roll = roll;
        if(!cdb->fetch_ex(tc_student))
        {
            delete tc_student;
            return;
        }
        interface->rollnoLe->setDisabled(true);
        interface->set_tc(tc_student);
        interface->btable->add_roll(tc_student->roll,"TC");
        interface->rollnoLe->clear();
        return;
    }


    student = new Student;
    student->c_type[0] = c_type[0];
    student->c_type[1] = c_type[1];
    student->roll = roll;

    if(d_mode)
    {
        // Database Mode
        if(!cdb->fetch(student))
        {
            delete student;
            return;
        }
    }


    // Make Entry into the table
    interface->btable->add_roll(student->roll,rtype);

    // Clear the Roll Box
    interface->rollnoLe->clear();
    interface->rollnoLe->setFocus();

    // Update the count of each certificates
    c_count.replace(0 ,c_count.at(0) + student->c_type[0]);
    interface->bcl->setText(temp.sprintf("%d",c_count.at(0)));
    c_count.replace(1 ,c_count.at(1) + student->c_type[1]);
    interface->ccl->setText(temp.sprintf("%d",c_count.at(1)));

    // Assign Serial No. to this entry (bonafide or conduct only)
    int i;
    for(i = 0; i < 2 ; ++i)
    {
        if(student->c_type[i])
            student->c_sno[i] = c_sno.at(i) + c_count.at(i);
    }
    // Append to the print List
    this->st_plist->append(student);

    // Set the interface
    interface->set_ct(student);
}

void AppManager::previewLastRoll()
{
    // To get a preview of the last Roll Number printed
}

/* itemchanged(int,const QString)
 * Called by each QGraphicTextItem upon change
 * in it Contents.
 */
void AppManager::itemchanged(int id, const QString &data)
{

    if(id < 0 )
    {
        interface->wframe->setEnabled(false);
        interface->larrow->setAcceptHoverEvents(false);
        interface->rarrow->setAcceptHoverEvents(false);
        et += id;
        p_lock -= 1;
        return;
    }

    switch(id)
    {
    case 1: if(c_mode)
                    tc_student->name =data;
                else
                    st_plist->at(st_ptr)->name = data;
        break;
    case 2: st_plist->at(st_ptr)->co = data;
        break;
    case 3: if(c_mode)
                    tc_student->father_name = data;
                else
                    st_plist->at(st_ptr)->father_name = data;
        break;
    case 4: st_plist->at(st_ptr)->cdetails = data;
        break;
    case 5: st_plist->at(st_ptr)->purpose = data;
        break;
    case 6: tc_student->std_ex->dob = data;
        break;
    case 7: tc_student->std_ex->doa = data;
        break;
    case 8: tc_student->std_ex->course = data;
        break;
    case 9: tc_student->std_ex->branch = data;
        break;
    case 10 : tc_student->std_ex->dol = data;
        break;
    case 11: tc_student->std_ex->qualified = data;
        break;
    case 12: tc_student->std_ex->dues = data;
        break;
    case 13: tc_student->std_ex->disp_m = data;
        break;
    case 14: tc_student->std_ex->conduct = data;
        break;
    case 15: tc_student->std_ex->community = data;
    }

    et += id;
    p_lock += 1;

    if(et == 0)
    {
        interface->wframe->setEnabled(true);
        interface->larrow->setAcceptHoverEvents(true);
        interface->rarrow->setAcceptHoverEvents(true);
    }
}

/* btableclicked(int,int)
 * Called when a btable cell is clicked
 * Sets the preview corresponding to the
 * Roll Number Clicked
 */
void AppManager::btableclicked(int row,int col)
{
    if(c_mode)
        return;
    student = st_plist->at(row);

    interface->set_ct(student);
    fg_rollLe = 0;
    interface->btable->st_ptr = st_ptr = row;
    interface->cstack->setStyleSheet(BTABLE_FRAME_COLOR);

    for(int i = 0;i < 2; ++i)
    {
        if(student->c_type[i])
            bg_ctype->button(i)->setChecked(true);
        else
            bg_ctype->button(i)->setChecked(false);
    }
}

/* updatetype(int)
 * Called when user changes the Certificate type
 * through checkboxes
 */
void AppManager::updatetype(int i)
{
    btype = "";
    rtype = "";

    if(!fg_rollLe) // Table update
    {

        if(bg_ctype->button(i)->isChecked())
        {
            student->c_type[i] = 1;
            update_counts(i,1);
        }
        else
        {
            student->c_type[i] = 0;
            update_counts(i,-1);
            // Atleast one type needs to be checked !
            if(student->c_type[0] + student->c_type[1] == 0)
            {
                //report error
                interface->report(INFO,"Atleast One Certificate needs to be Selected.");
                bg_ctype->button(i)->setChecked(true);
            }
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

        interface->btable->item(st_ptr,1)->setText(btype);


        interface->set_ct(student);
    }
    else
    {
        if(bg_ctype->button(i)->isChecked())
            c_type[i] = 1;
        else
        {
            c_type[i] = 0;

            // Atleast one type needs to be checked !
            if(c_type[0] + c_type[1] == 0)
            {
                //report error
                interface->report(INFO,"Atleast One Certificate needs to be Selected.");
                bg_ctype->button(i)->setChecked(true);
            }
        }

        for(int i = 0;i < 2; ++i)
        {
            if(c_type[i])
            {
                switch(i)
                {
                case 0 : rtype += "B ";
                    break;
                case 1 : rtype += "C ";
                }
            }
        }

    }
}

/* removeSt(int)
 * Called by btable when an roll number is deleted
 * by pressing "delete" key
 */
void AppManager::removeSt(int i)
{
    if(c_mode)
    {
        interface->rollnoLe->setDisabled(false);
        delete tc_student;
        return;
    }


    int j, k;
    // Update Counts
    c_count.replace(0, c_count.at(0) - st_plist->at(i)->c_type[0]);
    interface->bcl->setText(temp.sprintf("%d",c_count.at(0)));
    c_count.replace(1, c_count.at(1) - st_plist->at(i)->c_type[1]);
    interface->bcl->setText(temp.sprintf("%d",c_count.at(1)));


    // Update Serial No.
    for(j = 0 ; j < 2; ++j)
    {
        if(st_plist->at(i)->c_type[j])
        {
            for(k = i + 1; k < st_plist->count() ; ++k)
            {
                st_plist->at(k)->c_sno[j] -= 1;
            }
        }
    }

    // Release the memory & clean up the entry
    delete this->st_plist->at(i);
    this->st_plist->removeAt(i);
}

/* rollfocus()
 * Called when the user doubleclicks the rollnolLe
 */
void AppManager::rollfocus()
{
    //rollnoLe now has the focus
    interface->rollnoLe->setCursorPosition(0);
    fg_rollLe = 1;

    interface->cstack->setStyleSheet(FRAME_COLOR);
    interface->btable->clearSelection();

    int i;
    for(i = 0;i < 2;++i)
        bg_ctype->button(i)->setChecked(c_type[i]);

    for(i = 0;i < 2;++i)
    {
        if(bg_ctype->button(i)->isChecked())
            break;
    }

   interface->resetView(i);
}

/* Right & Left Navigation works only when
 * the user clicks on an entry in the btable
 */
void AppManager::toRight()
{
    // Make Sure btable entry is selected
    if(fg_rollLe)
        return;

    student->vpic = (student->vpic + 1) % 2;

    for(int j=0; j<2;++j)
    {
        if(student->c_type[student->vpic])
            break;
        student->vpic = (student->vpic + 1) % 2;
    }

    interface->show_ct(student->vpic);
}

void AppManager::toLeft()
{
    // Make Sure btable is selected
    if(fg_rollLe)
        return;

    interface->pic[student->vpic]->setOpacity(0.0);
    student->vpic = (student->vpic + 3) % 2;

    for(int j=0; j<2;++j)
    {
        if(student->c_type[student->vpic])
            break;
        student->vpic = (student->vpic + 3) % 2;
    }

    interface->show_ct(student->vpic);
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
        interface->prints->hlabel->setText("Bonafide Certificate Printing");
        interface->prints->tlabel->setText("Bonafide");
        break;
    case 1: // All Conduct
        interface->prints->hlabel->setText("Conduct Certificate Printing");
        interface->prints->tlabel->setText("Conduct");
    }

    cprinter->p_type = p_type = r;
}

void AppManager::b_print()
{
    if(!interface->btable->rowCount())
        return;
    update_print_type(0);
    init_print(0);
}

void AppManager::c_print()
{
    if(!interface->btable->rowCount())
        return;
    update_print_type(1);
    init_print(1);
}

/* print()
 * Called when user presses the print button
 */
void AppManager::print()
{
    if(!interface->btable->rowCount())
        return;
    init_print(p_type);
}

/* init_print()
 * Called by print(), b_print(), c_print(),t_print()
 * Performs mandatory initializations
 */
void AppManager::init_print(int lp_type)
{
    // printing is locked, an item is being changed
    if(p_lock < 0)
        return;

    if(c_mode)
    {
        if(tc_student->std_ex->is)
        {
            DIssueDialog *dis = new DIssueDialog(tc_student);
            connect(dis,SIGNAL(print()),this,SLOT(Tc_dissue()));
            return;
        }

        interface->prints->hlabel->setText("Transfer Certificate Printing");
        interface->prints->tlabel->setText("Transfer");
        interface->prints->clabel->setText("1");
        cprinter->tc_student = tc_student;
        cprinter->p_type = lp_type;

        if(tc_student->std_ex->is == 0)
        {
            if(cdb->log_tc(tc_student))
                return;
        }
    }
    else
    {
        interface->prints->clabel->setText(QString().sprintf("%d",c_count.at(lp_type)));
        cprinter->pList(st_plist);
     }



    // Show the Print Dialogue
    interface->prints->move(interface->width()/2 - 240 , interface->height()/2 - 94);
    interface->prints->show();
}

void AppManager::Tc_dissue()
{
    // replace dissue date to cdate

    cprinter->tc_student = tc_student;
    if(!cdb->update_dissue(tc_student))
        cprinter->printTC();
}

void AppManager::pcomplete(int ctype)
{
    
    if(ctype == 2)
    {
        interface->btable->clearContents();
        interface->rollnoLe->setDisabled(false);
        return;
    }

    for(int j=0; j<= st_plist->count();++j)
    {
        st_plist->at(j)->c_type[ctype] = 0;

        // Entry has no certificates
        if(st_plist->at(j)->c_type[!ctype]  == 0)
        {
            delete st_plist->at(j);
            st_plist->removeAt(j);
            interface->btable->removeRow(j);
            --j;
        }
    }

    c_count.replace(ctype , 0);
}



void AppManager::updatevalues(int i)
{
    switch(i)
    {
    case 0 :
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;

    }

}

void AppManager::update_counts(int ctype, int c)
{
    c_count.replace(ctype ,c_count.at(ctype) + c);

    if(ctype)
        interface->ccl->setText(temp.sprintf("%d",c_count.at(ctype)));
    else
        interface->bcl->setText(temp.sprintf("%d",c_count.at(ctype)));

    // Updating Serial Numbers of this type
    if(ctype == 2)
        return;
    else
    {
        for(int i = st_ptr + 1 ; i < interface->btable->rc ; ++i)
            st_plist->at(i)->c_sno[ctype] += c;
    }
}

void AppManager::modechange(int i)
{
    // Database Mode : mode = 1
    // Manual Mode : mode = 0
    d_mode = i;
    if(i)
        interface->mode->setText("Mode : " + tr("<b>") + "Database" + tr("<b>"));
    else
        interface->mode->setText("Mode : " + tr("<b>") + "Manual" + tr("<b>"));
}

void AppManager::TC_mode()
{
    QString btype;
    c_mode = !c_mode;

    // TC Modes requires application to be in DB Mode
    if(c_mode == 1 and d_mode == 0)
    {
        interface->report(ERROR, "TC Can be only issued in Database Mode.");
        c_mode = 0;
        return;
    }

    switch(c_mode)
    {
    case 0 : // Set to Normal Mode
                interface->set_TC_Items(0);
                interface->tcb->setText("Issue TC");
                interface->rbb->setChecked(true);
                interface->cstack->setCurrentIndex(0);
                interface->bcgroup->setDisabled(false);

                // Check if st_plist is empty.
                if(!st_plist->isEmpty())
                {
                    for(int i = 0 ; i < st_plist->count() ; ++i)
                    {
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

                        interface->btable->add_roll(student->roll,rtype);
                        btype = "";
                    }
                }

                p_type = 0;
                break;

    case 1: // Set to TC Mode
                interface->cstack->setCurrentIndex(1);
                interface->tcb->setText("Close TC");
                interface->set_TC_Items(1);
                interface->cdate->setEnabled(false);
                interface->btable->clearContents();
                interface->bcgroup->setDisabled(true);
                interface->btable->rc = 0;
                p_type = 2;

                connect(interface->doa,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->dob,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->dol,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->course,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->branch,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->qlf,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->dues,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->dis,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->conduct,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
                connect(interface->community,SIGNAL(itemchanged(int ,const QString &)),this,SLOT(itemchanged(int ,const QString &)));
    }
}

void AppManager::topdf()
{
    qDebug() << "to pdf";
}


void AppManager::quit()
{
    // Check if any entries in btable
    if(!st_plist->isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setParent(interface);
        msgBox.setWindowFlags(Qt::Window);
        msgBox.setText("They are Roll Entries waiting for Printing");
        msgBox.setInformativeText("Do you want to print them ?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int ret = msgBox.exec();

        switch(ret)
        {
        case QMessageBox::Yes :
            break;
        case QMessageBox::No : qApp->quit();
        }
    }
    else
        qApp->quit();
}


AppManager::~AppManager()
{

}
