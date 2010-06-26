#include "interface.h"
#include <QDebug>

Interface::Interface()
{
    setupUi(this);

    menu = new ButtonItem(QPixmap(":/Images/Images/menu.png"));
    menu->setAcceptHoverEvents(true);
    session = new ButtonItem(QPixmap(":/Images/Images/session.png"));
    session->setAcceptHoverEvents(true);

    larrow = new ArrowItem(QPixmap(":/Images/Images/larrow.png"));
    larrow->setAcceptHoverEvents(true);
    rarrow = new ArrowItem(QPixmap(":/Images/Images/rarrow.png"));
    rarrow->setAcceptHoverEvents(true);
    uarrow = new ArrowItem(QPixmap(":/Images/Images/uparrow.png"));
    uarrow->setAcceptHoverEvents(true);
    darrow = new ArrowItem(QPixmap(":/Images/Images/dnarrow.png"));
    darrow->setAcceptHoverEvents(true);

    menu->setPos(CView->mapToScene(QPoint(0,0)));
    session->setPos(CView->mapToScene(QPoint(CView->rect().width() - 87,0)));

    create_NItems();
    createPrintDialog();
    createShortCuts();
    this->showMaximized();
    createPanels();

    CView->verticalScrollBar()->setFocusPolicy(Qt::NoFocus);

    btable->setColumnWidth(0,158);
    btable->setColumnWidth(1,68);

    connect(CView, SIGNAL(viewresized()),this,SLOT(viewresized()));
    connect(CView,SIGNAL(viewscrolled()),this,SLOT(viewresized()));
    connect(CView->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(viewresized()));
    connect(fscreenb,SIGNAL(pressed()),this,SLOT(fnscreen()));
    connect(aboutb,SIGNAL(pressed()),this,SLOT(about_docmaq()));
}

void Interface::create_NItems()
{
    cscene = new CredentialScene;
    cscene->setSceneRect(0,0,619,438);

    pic[2] = new gpix(QPixmap("./tc.png"));
    pic[1] = new gpix(QPixmap("./conduct.png"));
    pic[1]->setZValue(pic[2]->zValue() + 1);
    pic[0] = new gpix(QPixmap("./bpicrz.png"));
    pic[0]->setZValue(pic[1]->zValue() + 1);

    z = pic[0]->zValue() + 1;

    QString t = QDate::currentDate().toString("dd.MM.yyyy");
    cdate = new QGraphicsTextItem;
    cdate->setPlainText(t);
    cdate->setPos(QPointF(512,132));
    cdate->setZValue(z);

    stname = new LineItem(cscene,328,261);
    stname->id = 1;
    stname->itemName("STUDENT NAME");
    stname->setPos(QPointF(230,191));
    stname->setZValue(z);

    co = new ComboItem(cscene,pic[0],140,69,QStringList() << "S/o" << "D/o");
    co->id = 2;
    co->setZValue(z);

    fname = new LineItem(cscene,328,261);
    fname->id = 3;
    fname->itemName("FATHER NAME");
    fname->setPos(QPointF(115,216));
    fname->setZValue(z);

    roll = new QGraphicsTextItem;
    roll->setPos(QPointF(206,244));
    roll->setZValue(z);

    cdetails = new LineItem(cscene,388,331);
    cdetails->id = 4;
    cdetails->itemName("COURSE DETAILS");
    cdetails->setPos(273,271);
    cdetails->setZValue(z);

    fyear = new QGraphicsTextItem(pic[0]);
    fyear->setPos(QPointF(192,300));
    fyear->setZValue(z);

    tyear = new QGraphicsTextItem(pic[0]);
    tyear->setPos(QPointF(294,300));
    tyear->setZValue(z);

    purposetext = new QGraphicsTextItem(pic[0]);
    purposetext->setPlainText("This Certificate is issued for the purpose of:");
    purposetext->setPos(QPointF(30,339));
    purposetext->setZValue(z);

    purpose = new LineItem(cscene,150,92,pic[0]);
    purpose->id = 5;
    purpose->itemName("PURPOSE");
    purpose->setPos(QPointF(365,339));
    purpose->setZValue(z);

    QFont snofont("Bitstream Vera Serif",11);
    snofont.setBold(true);

    sno = new QGraphicsTextItem;
    sno->setZValue(z);
    sno->setDefaultTextColor(QColor("red"));
    sno->setFont(snofont);

    acyear =  new QGraphicsTextItem(pic[1]);
    acyear->setPos(222,264);
    acyear->setZValue(z);


    uarrow->setZValue(z);
    darrow->setZValue(z);
    cscene->addItem(menu);
    cscene->addItem(session);
    cscene->addItem(larrow);
    cscene->addItem(rarrow);
    cscene->addItem(uarrow);
    cscene->addItem(darrow);

    cscene->addItem(pic[0]);
    cscene->addItem(pic[1]);
    cscene->addItem(pic[2]);


    cscene->addItem(stname);
    cscene->addItem(fname);
    cscene->addItem(roll);
    cscene->addItem(cdetails);
    cscene->addItem(sno);

    cscene->addItem(cdate);
    /*scscene->addItem(fyear);
    cscene->addItem(tyear);
    cscene->addItem(purposetext);
    cscene->addItem(purpose);

    cscene->addItem(acyear);*/

    CView->setScene(cscene);
    CView->setBackgroundBrush(QBrush(QColor("grey")));

    pic[1]->setOpacity(0.0);
    pic[2]->setVisible(false);
}

void Interface::set_TC_Items(int s)
{
    switch(s)
    {
    case 0: // Remove TC Items
        cscene->removeItem(admno);
        cscene->removeItem(doa);
        cscene->removeItem(dob);
        cscene->removeItem(dol);
        cscene->removeItem(qlf);
        cscene->removeItem(conduct);
        cscene->removeItem(community);
        cscene->removeItem(dues);
        cscene->removeItem(dis);
        cscene->removeItem(course);
        cscene->removeItem(branch);



        showTc(false);
        delete doa;
        delete dob;
        delete dol;
        delete qlf;
        delete conduct;
        delete community;
        delete dues;
        delete dis;
        delete course;
        delete branch;

        cscene->setSceneRect(0,0,619,438);

        break;
    case 1: // Set TC Items
        QFont tfont("Bitstream Vera Serif",11);
        tfont.setBold(true);
        tc_type = new QGraphicsTextItem(pic[2]);
        tc_type->setPos(518,70);
        tc_type->setZValue(z);
        tc_type->setFont(tfont);
        tc_type->setDefaultTextColor(QColor("red"));

        admno = new QGraphicsTextItem(pic[2]);
        admno->setPos(384,149);
        admno->setZValue(z);

        doa = new DateItem(cscene,pic[2],7);
        doa->itemName("DATE OF ADMISSION");
        doa->setPos(319,314);
        doa->setZValue(z);

        dob = new DateItem(cscene,pic[2],6);
        dob->itemName("DATE OF BIRTH");
        dob->setPos(190,269);
        dob->setZValue(z);

        dol = new DateItem(cscene,pic[2],10);
        dol->itemName("DATE OF LEAVING");
        dol->setPos(285,440);
        dol->setZValue(z);

        course = new ComboItem(cscene,pic[2],164,102, QStringList()
        << "B.Tech"
        << "M.Tech"
        << "MBA"
        << "MCA"
        );
        course->id = 8;
        course->itemName("COURSE");
        course->setPos(239,352);
        course->setZValue(z);

        branch = new ComboItem(cscene,pic[2],164,102, QStringList()
        << "AM"
        << "Bio-Tech"
        << "CSE"
        << "ECE"
        << "ECM"
        << "EEE"
        << "EET"
        << "IT"
        << "Mech"
        << "BSCE"
        << "CAD-CAM"
        << "ECE"
        << "SE"
        << "S & H"
        << "VLSI" );
        branch->id = 9;
        branch->itemName("BRANCH");
        branch->setPos(254,398);
        branch->setZValue(z);

        qlf = new LineItem(cscene,328,261,pic[2]);
        qlf->id = 11;
        qlf->itemName("YES / NO");
        qlf->setPos(QPointF(230,191));
        qlf->setPos(402,480);
        qlf->setZValue(z);

        dues = new LineItem(cscene,328,261,pic[2]);
        dues->id = 12;
        dues->itemName("DUES");
        dues->setPos(QPointF(230,191));
        dues->setPos(136,524);
        dues->setZValue(z);

        dis = new LineItem(cscene,328,261,pic[2]);
        dis->id = 13;
        dis->itemName("DISCIPILINE");
        dis->setPos(QPointF(230,191));
        dis->setPos(321,567);
        dis->setZValue(z);

        conduct = new LineItem(cscene,328,261,pic[2]);
        conduct->id = 14;
        conduct->itemName("CONDUCT");
        conduct->setPos(QPointF(230,191));
        conduct->setPos(172,610);
        conduct->setZValue(z);

        community = new LineItem(cscene,328,261,pic[2]);
        community->id = 15;
        community->itemName("COMMUNITY");
        community->setPos(QPointF(230,191));
        community->setPos(389,654);
        community->setZValue(z);

        showTc(true);
        cscene->setSceneRect(0,0,619,877);
    }
}

void Interface::set_items(QStringList *dlist)
{
    /*tyear->setPlainText(dlist->at());
    fyear->setPlainText(dlist->at());
    cdate->setPlainText(dlist->at());
    acyear->setPlainText(dlist->at() + " - " + dlist->at());
    */
}

void Interface::createPanels()
{
    QPixmap pixmap(":/Images/Images/panel.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage(":/Images/Images/panel.png")));

    //lpanel
    lframe = new QWidget;
    lframe->setGeometry(0,0,87, 244);

    settingsb = new QToolButton(lframe);
    settingsb->setGeometry(QRect(10, 10, 71, 81));
    QFont font;
    font.setBold(true);
    font.setWeight(75);
    settingsb->setFont(font);
    settingsb->setText("Settings");
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Images/Images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
    settingsb->setIcon(icon);
    settingsb->setIconSize(QSize(48, 48));
    settingsb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    settingsb->setAutoRaise(true);
    settingsb->setArrowType(Qt::NoArrow);

    fscreenb = new QToolButton(lframe);
    fscreenb->setGeometry(QRect(10, 90, 71, 71));
    fscreenb->setFont(font);
    fscreenb->setText("Full Screen");
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/Images/Images/fullscreen.png"), QSize(), QIcon::Normal, QIcon::Off);
    fscreenb->setIcon(icon2);
    fscreenb->setIconSize(QSize(48, 48));
    fscreenb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fscreenb->setAutoRaise(true);

    tcb = new QToolButton(lframe);
    tcb->setGeometry(QRect(10, 170, 71, 71));
    tcb->setFont(font);
    tcb->setText("Issue TC");
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/Images/Images/tc.png"), QSize(), QIcon::Normal, QIcon::Off);
    tcb->setIcon(icon1);
    tcb->setIconSize(QSize(48, 48));
    tcb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    tcb->setAutoRaise(true);

    lframe->setMask(pixmap.mask());
    lframe->setPalette(palette);

    //rpanel
    rframe = new QWidget;
    rframe->setGeometry(0,0,87, 244);

    logb = new QToolButton(rframe);
    logb->setGeometry(QRect(10, 10, 71, 81));
    logb->setFont(font);
    logb->setText("Logs");
    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/Images/Images/log.png"), QSize(), QIcon::Normal, QIcon::Off);
    logb->setIcon(icon4);
    logb->setIconSize(QSize(48, 48));
    logb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    logb->setAutoRaise(true);
    logb->setArrowType(Qt::NoArrow);

    helpb = new QToolButton(rframe);
    helpb->setGeometry(QRect(10, 170, 71, 71));
    helpb->setFont(font);
    helpb->setText("Help");
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/Images/Images/help.png"), QSize(), QIcon::Normal, QIcon::Off);
    helpb->setIcon(icon5);
    helpb->setIconSize(QSize(48, 48));
    helpb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    helpb->setAutoRaise(true);

    aboutb = new QToolButton(rframe);
    aboutb->setGeometry(QRect(10, 90, 71, 71));
    aboutb->setFont(font);
    aboutb->setText("About");
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/Images/Images/printer.png"), QSize(), QIcon::Normal, QIcon::Off);
    aboutb->setIcon(icon6);
    aboutb->setIconSize(QSize(48, 48));
    aboutb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    aboutb->setAutoRaise(true);

    rframe->setMask(pixmap.mask());
    rframe->setPalette(palette);

    lproxy = cscene->addWidget(lframe);
    rproxy = cscene->addWidget(rframe);

    // Parallel Animations for Both Panels
    QParallelAnimationGroup *group = new QParallelAnimationGroup;
    anim = new QPropertyAnimation(lproxy,"pos");
    anim->setEasingCurve(QEasingCurve::InOutBack);
    anim->setDuration(500);
    group->addAnimation(anim);

    anim = new QPropertyAnimation(rproxy,"pos");
    anim->setEasingCurve(QEasingCurve::InOutBack);
    anim->setDuration(500);
    group->addAnimation(anim);

    machine  = new QStateMachine(this);
    s1 = new QState();
    s2 = new QState();

    QAbstractTransition *ts = s1->addTransition(menu, SIGNAL(pressed()), s2);
    ts->addAnimation(group);

    ts = s2->addTransition(menu, SIGNAL(pressed()), s1);
    ts->addAnimation(group);

    machine->addState(s1);
    machine->addState(s2);
    machine->setInitialState(s1);
    machine->start();
}

void Interface::createPrintDialog()
{
    prints = new SPrintDialog(this);
    prints->hide();
}

void Interface::createsession(QStringList lt)
{
    this->wframe->setEnabled(false);

    QPushButton *close;
    QLabel *unamecs,*ltimecs,*ctotal,*cb,*cc,*ct,*unamels, *ltimels,*lotimels;
    QWidget *widget = new QWidget;
    widget->setGeometry(0,0,555,564);

    close = new QPushButton(widget);
    close->setObjectName(QString::fromUtf8("close"));
    close->setGeometry(QRect(470, 10, 75, 23));
    unamecs = new QLabel(widget);
    unamecs->setGeometry(QRect(320, 130, 221, 21));
    unamecs->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ltimecs = new QLabel(widget);
    ltimecs->setGeometry(QRect(320, 170, 221, 21));
    ltimecs->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ctotal = new QLabel(widget);
    ctotal->setGeometry(QRect(450, 215, 91, 21));
    ctotal->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    cb = new QLabel(widget);
    cb->setGeometry(QRect(450, 260, 91, 21));
    cb->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    cc = new QLabel(widget);
    cc->setGeometry(QRect(450, 300, 91, 21));
    cc->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ct = new QLabel(widget);
    ct->setGeometry(QRect(450, 340, 91, 21));
    ct->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    unamels = new QLabel(widget);
    unamels->setGeometry(QRect(350, 440, 191, 21));
    unamels->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ltimels = new QLabel(widget);
    ltimels->setGeometry(QRect(350, 480, 191, 21));
    ltimels->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    lotimels = new QLabel(widget);
    lotimels->setGeometry(QRect(350, 520, 191, 21));
    lotimels->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    QPixmap pixmap("./dhome.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("./dhome.png")));
    widget->setMask(pixmap.mask());
    widget->setPalette(palette);
    sproxy = new QGraphicsProxyWidget;

    sproxy = cscene->addWidget(widget);

    // Gather Session Info form settings

    connect(close,SIGNAL(pressed()),this,SLOT(closesessionpage()));
}

// createShortCuts()
// Key Board ShortCuts
void Interface::createShortCuts()
{
    Bsc = new QShortcut(QKeySequence(tr("F1")),this);
    Csc = new QShortcut(QKeySequence(tr("F2")),this);
    help = new QShortcut(QKeySequence(tr("Ctrl + h")),this);
}

void Interface::setStatusBar(QString user, int c)
{
    QLabel *ulb = new QLabel;
    QLabel *timelb = new QLabel;
    mode = new QLabel;

    ulb->setText("User Logged In : " + tr("<b>") + user + "<b>");
    timelb->setText("Login Time : " + tr("<b>") + QDateTime::currentDateTime().toString() + "<b>");
    if(c)
        mode->setText("Mode : " + tr("<b>") + "Database" + tr("<b>"));
    else
        mode->setText("Mode : " + tr("<b>") + "Manual" + tr("<b>"));

    timelb->setAlignment(Qt::AlignCenter);
    mode->setAlignment(Qt::AlignRight);

    sBar->addWidget(ulb,1);
    sBar->addWidget(timelb,1);
    sBar->addWidget(mode,1);
}

/* void set_ct(Student *)
 * Called to fill the student details with Line Items
 * suitable only in DB mode
 */
void Interface::set_ct(Student *student)
{
    // Sets the Details in the certificate
    // In DB mode, st_filled is mostly 1
    // In Manual mode st_filled is always 0
   if(student->st_filled)
   {
       stname->setPlainText(student->name);
       fname->setPlainText(student->father_name);
       roll->setPlainText(student->roll);
       co->setPlainText(student->co);

       cdetails->setPlainText(student->cdetails);
       purpose->setPlainText(student->purpose);

       if(student->c_type[0])
          sno->setPlainText(temp.sprintf("%04d",student->c_sno[0]));
       else
          sno->setPlainText(temp.sprintf("%04d",student->c_sno[1]));
    }
   else
   {
       stname->setText(student->name);
       fname->setText(student->father_name);
       roll->setPlainText(student->roll);
       co->setPlainText(student->co);

       cdetails->setText(student->cdetails);
       purpose->setPlainText(student->purpose);

       if(student->c_type[0])
          sno->setPlainText(temp.sprintf("%04d",student->c_sno[0]));
       else
          sno->setPlainText(temp.sprintf("%04d",student->c_sno[1]));
   }

   int i;
   for(i=0; i < 2; ++i)
       if(student->c_type[i])
           break;

   student->vpic = i;
   show_ct(i);
}

void Interface::set_tc(Student *student)
{
    stname->setPlainText(student->name);
    fname->setPlainText(student->father_name);
    roll->setPlainText(student->roll);

    stname->setPos(214,186);
    fname->setPos(202,228);
    roll->setPos(82,149);
    cdate->setPos(542,149);

   // tc_type->setPlainText(student->std_ex->type);
    admno->setPlainText(student->roll);
    dob->setText(student->std_ex->dob);
    doa->setText(student->std_ex->doa);
    course->setText(student->course);
    branch->setText(student->branch);

    dol->setText(student->std_ex->dol);
    qlf->setPlainText(student->std_ex->qualified);
    dues->setPlainText(student->std_ex->dues);
    dis->setPlainText(student->std_ex->disp_m);
    conduct->setPlainText(student->std_ex->conduct);
    community->setPlainText(student->std_ex->community);
}

void Interface::show_ct(int vpic)
{
    switch(vpic)
    {
    case 0: // Bonafide
        pic[0]->setOpacity(1.0);
        pic[1]->setOpacity(0.0);

        stname->setPos(230,191);
        fname->setPos(115,216);
        roll->setPos(206,244);
        cdate->setPos(515,132);
        co->setPos(505,191);
        cdetails->setPos(273,271);
        sno->setPos(168,131);
        break;
    case 1: // Conduct
        pic[0]->setOpacity(0.0);
        pic[1]->setOpacity(1.0);

        cdate->setPos(520,59);
        stname->setPos(164,143);
        co->setPos(502,143);
        fname->setPos(57,175);
        roll->setPos(152,205);
        cdetails->setPos(227,235);
        sno->setPos(100,100);

      }
}

void Interface::resetView(int i)
{

    stname->setPlainText("");
    fname->setPlainText("");
    roll->setPlainText("");
    co->setPlainText("");
    sno->setPlainText("");
    cdetails->setPlainText("");
    purpose->setPlainText("");

    show_ct(i);
}

void Interface::showTc(bool var)
{
    admno->setVisible(var);
    dob->setVisible(var);
    doa->setVisible(var);
    course->setVisible(var);
    branch->setVisible(var);
    dol->setVisible(var);
    qlf->setVisible(var);
    dues->setVisible(var);
    dis->setVisible(var);
    conduct->setVisible(var);
    community->setVisible(var);
    pic[2]->setVisible(var);

    var = !var;

    stname->setVisible(var);
    fname->setVisible(var);
    co->setVisible(var);
    cdetails->setVisible(var);
    sno->setVisible(var);
    tyear->setVisible(var);
    fyear->setVisible(var);
    acyear->setVisible(var);

    larrow->setVisible(var);
    rarrow->setVisible(var);
    darrow->setVisible(var);
    uarrow->setVisible(var);

    pic[0]->setVisible(var);
    pic[1]->setVisible(var);
}

void Interface::viewresized()
{
    int w = CView->viewport()->width();
    int h = CView->viewport()->height();
    int mw = w/2;
    int mh = h / 2 - 70;

    // re position menu and session buttons
    menu->setPos(CView->mapToScene(CView->viewport()->pos()));
    session->setPos(CView->mapToScene(QPoint(w - 87,0)));

    // re position the arrow buttons
    larrow->setPos(CView->mapToScene(0,mh));
    rarrow->setPos(CView->mapToScene(w - 21,mh));
    uarrow->setPos(CView->mapToScene(mw - 145, h - 20));
    darrow->setPos(CView->mapToScene(mw + 5, h - 20));

    // re postion panels
    QPointF pt,pt2;
    pt = CView->mapToScene(QPoint(-87,h - 241));
    s1->assignProperty(lproxy,"pos",pt);

    pt2 = CView->mapToScene(QPoint(w, h - 241));
    s1->assignProperty(rproxy,"pos",pt2);

    if(machine->configuration().contains(s1))
    {
        lproxy->setPos(pt);
        rproxy->setPos(pt2);
    }

    pt.setX(pt.x() + 87);
    s2->assignProperty(lproxy,"pos",pt);

    pt2.setX(pt2.x() - 87);
    s2->assignProperty(rproxy,"pos",pt2);

    if(machine->configuration().contains(s2))
    {
        lproxy->setPos(pt);
        rproxy->setPos(pt2);
    }
}

void Interface::fnscreen()
{
    if(this->isFullScreen())
    {
        this->showMaximized();
        fscreenb->setText("Full Screen");
    }
    else
    {
        this->showFullScreen();
        fscreenb->setText("Normal");
    }
}

void Interface::report(int type, QString msg)
{
    switch(type)
    {
    case 0 : // Error
        QMessageBox::critical (this,"DocmaQ Error",msg);
        break;
    case 1: // Warning
        QMessageBox::warning(this,"DocmaQ Warning",msg);
        break;
    case 2: // Information
        QMessageBox::information(this,"DocmaQ Information",msg);
    }
}

void Interface::closesessionpage()
{
    cscene->removeItem(sproxy);
    this->wframe->setEnabled(true);
    this->rollnoLe->setFocus();
}

void Interface::about_docmaq()
{
    About_DocmaQ *abt = new About_DocmaQ(this);
    abt->show();
}

void Interface::closeEvent(QCloseEvent *event)
{
   event->ignore();
   emit quit();
}
