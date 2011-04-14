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

#include "interface.h"
#include <QDebug>

/*
 * Constructor
 */
Interface::Interface()
{
    setupUi(this);

    menu = new HoverItem(QPixmap(":/Images/menu.png"));
    menu->enable(true);
    session = new HoverItem(QPixmap(":/Images/session.png"));
    session->enable(true);

    larrow = new HoverItem(QPixmap(":/Images/larrow.png"));
    larrow->enable(true);
    rarrow = new HoverItem(QPixmap(":/Images/rarrow.png"));
    rarrow->enable(true);
    uarrow = new HoverItem(QPixmap(":/Images/uparrow.png"));
    uarrow->enable(true);
    darrow = new HoverItem(QPixmap(":/Images/dnarrow.png"));
    darrow->enable(true);

    up_stp = new QGraphicsRectItem();
    up_stp->setBrush(QBrush(QColor("black")));

    dn_stp = new QGraphicsRectItem();
    dn_stp->setBrush(QBrush(QColor("black")));

    menu->setPos(CView->mapToScene(QPoint(0,0)));
    session->setPos(CView->mapToScene(QPoint(CView->rect().width() - 87,0)));

    create_NItems();
    createPrintDialog();
    createShortCuts();
    setStatusBar();
    createPanels();

    btable->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
    btable->verticalHeader()->setResizeMode(QHeaderView::Fixed);

    CView->verticalScrollBar()->setFocusPolicy(Qt::NoFocus);

    btable->setColumnWidth(0,158);
    btable->setColumnWidth(1,68);

    connect(CView, SIGNAL(viewresized()),this,SLOT(viewresized()));
    connect(CView,SIGNAL(viewscrolled()),this,SLOT(viewresized()));
    connect(CView->verticalScrollBar(),SIGNAL(sliderMoved(int)),this,SLOT(viewresized()));
    connect(fscreenb,SIGNAL(pressed()),this,SLOT(fnscreen()));
    connect(aboutb,SIGNAL(pressed()),this,SLOT(about_docmaq()));

    tpl = false;
}

/* create_NItems
 * Called : By Interface()
 * Performs : Initialization of Graphic Items
 */
void Interface::create_NItems()
{
    cscene = new QGraphicsScene;
    cscene->setSceneRect(0,0,619,438);
    QFont ifont("Bitstream Vera Serif",10);
    ifont.setBold(true);

    pic[2] = new QGraphicsPixmapItem(QPixmap("./images/tc.png"));
    pic[1] = new QGraphicsPixmapItem(QPixmap("./images/conduct.png"));
    pic[1]->setZValue(pic[2]->zValue() + 1);
    pic[0] = new QGraphicsPixmapItem(QPixmap("./images/bonafide.png"));
    pic[0]->setZValue(pic[1]->zValue() + 1);

    z = pic[0]->zValue() + 1;

    QFont snofont("Bitstream Vera Serif",11);
    snofont.setBold(true);

    sno = new QGraphicsTextItem;
    sno->setZValue(z);
    sno->setDefaultTextColor(QColor("red"));
    sno->setFont(snofont);

    cdate = new QGraphicsTextItem;
    cdate->setFont(ifont);
    cdate->setPos(512,129);
    cdate->setZValue(z);

    stname = new LineItem(cscene,241);
    stname->setFont(ifont);
    stname->id = 1;
    stname->itemName("STUDENT NAME");
    stname->setPos(230,191);
    stname->setZValue(z);

    co = new ComboItem(cscene,69,QStringList() << "S/o" << "D/o");
    co->setFont(ifont);
    co->id = 2;
    co->setZValue(z);

    fname = new LineItem(cscene,241);
    fname->setFont(ifont);
    fname->id = 3;
    fname->itemName("FATHER NAME");
    fname->setPos(QPointF(115,216));
    fname->setZValue(z);

    roll = new QGraphicsTextItem;
    roll->setFont(ifont);
    roll->setPos(206,244);
    roll->setZValue(z);

    cdetails = new LineItem(cscene,240);
    cdetails->setFont(ifont);
    cdetails->id = 4;
    cdetails->itemName("COURSE DETAILS");
    cdetails->setPos(273,271);
    cdetails->setZValue(z);

    fyear = new QGraphicsTextItem(pic[0]);
    fyear->setFont(ifont);
    fyear->setPos(192,300);
    fyear->setZValue(z);

    tyear = new QGraphicsTextItem(pic[0]);
    tyear->setFont(ifont);
    tyear->setPos(QPointF(294,300));
    tyear->setZValue(z);

    purposetext = new QGraphicsTextItem(pic[0]);
    purposetext->setFont(ifont);
    purposetext->setPlainText("This Certificate is issued for the purpose of :");
    purposetext->setPos(30,339);
    purposetext->setZValue(z);

    purpose = new LineItem(cscene,92,pic[0]);
    purpose->setFont(ifont);
    purpose->id = 5;
    purpose->itemName("PURPOSE");
    purpose->setPos(345,339);
    purpose->setZValue(z);

    acyear = new LineItem(cscene,92,pic[1]);
    acyear->setFont(ifont);
    acyear->id = 6;
    acyear->itemName("YYYY - YYYY");
    acyear->setPos(222,262);
    acyear->setZValue(z);

    up_stp->setZValue(z);
    dn_stp->setZValue(z);

    int uz = up_stp->zValue() + 1;
    int dz = up_stp->zValue() + 1;

    menu->setZValue(uz);
    session->setZValue(uz);
    uarrow->setZValue(dz);
    darrow->setZValue(dz);
    larrow->setZValue(dz);
    rarrow->setZValue(dz);

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
    cscene->addItem(co);

    cscene->addItem(up_stp);
    cscene->addItem(dn_stp);

    CView->setScene(cscene);
    CView->setBackgroundBrush(QBrush(QColor("grey")));

    pic[1]->setOpacity(0.0);
    pic[2]->setVisible(false);
}

/* set_TC_Items(int)
 * Called : By AppManager::TC_mode()
 * Performs : Initialization/Removal of Graphic Items of TC
 */
void Interface::set_TC_Items(int s)
{
    switch(s)
    {
    case 0:
        typecb->disconnect();
        sersb->disconnect();

        stname->setAcceptHoverEvents(true);
        fname->setAcceptHoverEvents(true);

        // Remove TC Items
        for(int i = 0 ; i < 11 ; ++i)
        {
            cscene->removeItem(tcitems->at(i));
            delete tcitems->at(i);
        }

        tcitems->clear();
        delete tcitems;
        delete tc_type;
        delete tc_sno;
        delete admno;

        showTc(false);
        cscene->removeItem(tpanel);
        cscene->setSceneRect(0,0,619,438);

        tpl = false;
        break;
    case 1:
        stname->setAcceptHoverEvents(false);
        fname->setAcceptHoverEvents(false);

        // Set TC Items
        tcitems = new QList <QGraphicsTextItem *>;

        QFont tfont("Bitstream Vera Serif",11);
        tfont.setBold(true);

        tc_sno = new QGraphicsTextItem(pic[2]);
        tc_sno->setPos(45,62);
        tc_sno->setZValue(z);
        tc_sno->setFont(tfont);
        tc_sno->setDefaultTextColor(QColor("red"));

        tc_type = new QGraphicsTextItem(pic[2]);
        tc_type->setPos(518,62);
        tc_type->setZValue(z);
        tc_type->setFont(tfont);
        tc_type->setDefaultTextColor(QColor("red"));

        QFont ifont("Bitstream Vera Serif",10);
        ifont.setBold(true);

        admno = new QGraphicsTextItem(pic[2]);
        admno->setPos(384,149);
        admno->setZValue(z);
        admno->setFont(ifont);

        // DATE OF BIRTH
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(0)->setPos(294,269);
        tcitems->at(0)->setZValue(z);
        tcitems->at(0)->setFont(ifont);

        // DATE OF ADMISSION
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(1)->setPos(362,312);
        tcitems->at(1)->setZValue(z);
        tcitems->at(1)->setFont(ifont);

        // COURSE
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(2)->setPos(332,352);
        tcitems->at(2)->setZValue(z);
        tcitems->at(2)->setFont(ifont);

        // BRANCH
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(3)->setPos(254,398);
        tcitems->at(3)->setZValue(z);
        tcitems->at(3)->setFont(ifont);

        // DATE OF LEAVING
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(4)->setPos(345,440);
        tcitems->at(4)->setZValue(z);
        tcitems->at(4)->setFont(ifont);

        // YES / NO
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(5)->setPos(414,480);
        tcitems->at(5)->setZValue(z);
        tcitems->at(5)->setFont(ifont);

        // DUES
        tcitems->append(new QGraphicsTextItem(pic[2]));    
        tcitems->at(6)->setPos(304,524);
        tcitems->at(6)->setZValue(z);
        tcitems->at(6)->setFont(ifont);

        // DISCIPILINE
        tcitems->append(new QGraphicsTextItem(pic[2]));   
        tcitems->at(7)->setPos(321,567);
        tcitems->at(7)->setZValue(z);
        tcitems->at(7)->setFont(ifont);

        ifont.setItalic(true);

        // CONDUCT
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(8)->setPos(266,610);
        tcitems->at(8)->setZValue(z);
        tcitems->at(8)->setFont(ifont);

        ifont.setItalic(false);

        // COMMUNITY
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(9)->setPos(389,654);
        tcitems->at(9)->setZValue(z);
        tcitems->at(9)->setFont(ifont);

        // REMARKS
        tcitems->append(new QGraphicsTextItem(pic[2]));
        tcitems->at(10)->setPos(220,698);
        tcitems->at(10)->setZValue(z);
        tcitems->at(10)->setFont(ifont);

        stname->setPlainText("");
        fname->setPlainText("");
        roll->setPlainText("");

        showTc(true);
        stname->setPos(214,183);
        fname->setPos(202,225);
        roll->setPos(82,149);
        cdate->setPos(538,149);

        create_TC_Panel();
        cscene->setSceneRect(0,0,619,877);

        int w = CView->viewport()->width() / 2;
        int h = CView->viewport()->height();

        tpanel->setPos(CView->mapToScene(w - 215, h - 50));

        tpl = true;
    }
}

/* set_items(QStringList &)
 * Called : By Interface()
 * Performs : Data initialization of Items
 */
void Interface::set_items(QStringList &dlist)
{
    cdate->setPlainText(dlist.at(0));
    fyear->setPlainText(dlist.at(1));
    tyear->setPlainText(dlist.at(2));
}

/* create_TC_Panels()
 * Called : in Interface()
 * Performs : Creates the TC Panel
 */
void Interface::create_TC_Panel()
{
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QWidget *widget = new QWidget;

    QPixmap pixmap(":/Images/tpanel.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage(":/Images/tpanel.png")));

    widget->setGeometry(0,0,430, 50);
    label = new QLabel(widget);
    label->setGeometry(QRect(36, 1, 81, 16));
    label->setText("ROLL NUMBER");

    troll = new QLineEdit(widget);
    troll->setGeometry(QRect(10, 20, 131, 25));
    QFont font;
    font.setPointSize(10);
    font.setBold(true);
    troll->setFont(font);

    sersb = new QSpinBox(widget);
    sersb->setGeometry(QRect(150, 20, 71, 25));
    sersb->setMinimum(1);
    sersb->setMaximum(99999);
    sersb->setValue(1);
    sersb->setEnabled(false);

    typecb = new QCheckBox(widget);
    typecb->setText("ORIGINAL");
    typecb->setEnabled(false);
    typecb->setGeometry(QRect(236, 20, 81, 21));

    tprint = new QPushButton(widget);
    tprint->setGeometry(QRect(330, 9, 91, 34));

    QFont font1;
    font1.setFamily(QString::fromUtf8("Segoe UI"));
    font1.setPointSize(12);
    font1.setBold(true);
    font1.setWeight(75);

    QIcon icon;
    icon.addFile(":/Images/docmaq.png",QSize(),QIcon::Normal, QIcon::Off);
    tprint->setFont(font1);
    tprint->setText("Print");
    tprint->setCursor(QCursor(Qt::PointingHandCursor));
    tprint->setIcon(icon);

    label_2 = new QLabel(widget);
    label_2->setGeometry(QRect(157, 1, 61, 16));
    label_2->setText("SERIAL NO.");

    label_3 = new QLabel(widget);
    label_3->setGeometry(QRect(258, 1, 41, 16));
    label_3->setText("TYPE");

    widget->setMask(pixmap.mask());
    widget->setPalette(palette);

    tpanel = cscene->addWidget(widget);
    tpanel->setZValue(z);
}

/* createPanels()
 * Called : By Interface()
 * Performs : Initialisation of Interface Panels
 */
void Interface::createPanels()
{
    QPixmap pixmap(":/Images/panel.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage(":/Images/panel.png")));

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
    icon.addFile(":/Images/settings.png", QSize(), QIcon::Normal, QIcon::Off);
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
    icon2.addFile(":/Images/fullscreen.png", QSize(), QIcon::Normal, QIcon::Off);
    fscreenb->setIcon(icon2);
    fscreenb->setIconSize(QSize(48, 48));
    fscreenb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fscreenb->setAutoRaise(true);

    tcb = new QToolButton(lframe);
    tcb->setGeometry(QRect(10, 170, 71, 71));
    tcb->setFont(font);
    tcb->setText("Issue TC");
    QIcon icon1;
    icon1.addFile(":/Images/tc.png", QSize(), QIcon::Normal, QIcon::Off);
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
    icon4.addFile(":/Images/log48.png", QSize(), QIcon::Normal, QIcon::Off);
    logb->setIcon(icon4);
    logb->setIconSize(QSize(48, 48));
    logb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    logb->setAutoRaise(true);
    logb->setArrowType(Qt::NoArrow);

    pdfb = new QToolButton(rframe);
    pdfb->setGeometry(QRect(10, 90, 71, 71));
    pdfb->setFont(font);
    pdfb->setText("Print PDF");
    QIcon icon5;
    icon5.addFile(":/Images/pdf.png", QSize(), QIcon::Normal, QIcon::Off);
    pdfb->setIcon(icon5);
    pdfb->setIconSize(QSize(48, 48));
    pdfb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    pdfb->setAutoRaise(true);

    aboutb = new QToolButton(rframe);
    aboutb->setGeometry(QRect(10, 170, 71, 71));
    aboutb->setFont(font);
    aboutb->setText("About");
    QIcon icon6;
    icon6.addFile(":/Images/printer.png", QSize(), QIcon::Normal, QIcon::Off);
    aboutb->setIcon(icon6);
    aboutb->setIconSize(QSize(48, 48));
    aboutb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    aboutb->setAutoRaise(true);

    rframe->setMask(pixmap.mask());
    rframe->setPalette(palette);

    lproxy = cscene->addWidget(lframe);
    rproxy = cscene->addWidget(rframe);

    lproxy->setZValue(z + 10);
    rproxy->setZValue(z + 10);

    // Roll Display frame
    rollframe = new QWidget;  
    rollframe->setGeometry(0,0,178,32);
    rollDisp = new QLabel(rollframe);
    rollDisp->setGeometry(QRect(9, 0, 160, 30));
    QFont font2;
    font2.setFamily(QString::fromUtf8("Verdana"));
    font2.setPointSize(17);
    font2.setBold(true);
    font2.setWeight(75);
    rollDisp->setFont(font2);
    rollDisp->setAlignment(Qt::AlignCenter);

    QPalette p;
    p.setColor(QPalette::Background,Qt::black);
    rollframe->setPalette(p);
    rollproxy = cscene->addWidget(rollframe);
    rollproxy->setZValue(up_stp->zValue() + 1);

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

/*
 * Sets up the Print Dialog
 */
void Interface::createPrintDialog()
{
    prints = new SPrintDialog(this);
    prints->hlabel->setText("<font color='white'>Bonafide Certificate Printing</font>");
    prints->hide();
}

/* Interface::showsessionpage(QStringList &)
 * Called : By AppManager::createSessionPage()
 * Performs : Sets up the Session info in View
 */
void Interface::showsessionpage(QStringList &sinfo)
{
    QPushButton *close;
    QLabel *unamecs,*ltimecs,*cb,*cc,*ct,*unamels, *ltimels,*lotimels;
    QLabel *blast,*clast,*tlast;
    QFont sfont("Segoe UI",11);
    sfont.setBold(true);

    QWidget *widget = new QWidget;
    widget->setFont(sfont);
    widget->setGeometry(0,0,555,564);

    close = new QPushButton(widget);
    close->setText("Close");
    close->setGeometry(QRect(470, 10, 75, 23));
    sfont.setBold(false);
    close->setFont(sfont);

    unamecs = new QLabel(widget);
    unamecs->setText(sinfo.at(0));
    unamecs->setGeometry(QRect(320, 130, 221, 21));
    unamecs->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    ltimecs = new QLabel(widget);
    ltimecs->setText(sinfo.at(1));
    ltimecs->setGeometry(QRect(320, 170, 221, 21));
    ltimecs->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    cb = new QLabel(widget);
    cb->setText(sinfo.at(2));
    cb->setGeometry(QRect(450, 260, 91, 21));
    cb->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    blast = new QLabel(widget);
    blast->setText(sinfo.at(3));
    blast->setObjectName(QString::fromUtf8("blast"));
    blast->setGeometry(QRect(280, 260, 121, 21));
    blast->setAlignment(Qt::AlignCenter);

    cc = new QLabel(widget);
    cc->setText(sinfo.at(4));
    cc->setGeometry(QRect(450, 300, 91, 21));
    cc->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    clast = new QLabel(widget);
    clast->setText(sinfo.at(5));
    clast->setObjectName(QString::fromUtf8("clast"));
    clast->setGeometry(QRect(280, 300, 121, 21));
    clast->setAlignment(Qt::AlignCenter);

    ct = new QLabel(widget);
    ct->setText(sinfo.at(6));
    ct->setGeometry(QRect(450, 340, 91, 21));
    ct->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    tlast = new QLabel(widget);
    tlast->setText(sinfo.at(7));
    tlast->setGeometry(QRect(280, 340, 121, 21));
    tlast->setAlignment(Qt::AlignCenter);

    unamels = new QLabel(widget);
    unamels->setText(sinfo.at(8));
    unamels->setGeometry(QRect(350, 440, 191, 21));
    unamels->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    ltimels = new QLabel(widget);
    ltimels->setText(sinfo.at(9));
    ltimels->setGeometry(QRect(350, 480, 191, 21));
    ltimels->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    lotimels = new QLabel(widget);
    lotimels->setText(sinfo.at(10));
    lotimels->setGeometry(QRect(350, 520, 191, 21));
    lotimels->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

    QPixmap pixmap("./Images/session.png");
    QPalette palette;
    palette.setBrush(this->backgroundRole(), QBrush(QImage("./Images/session.png")));
    widget->setMask(pixmap.mask());
    widget->setPalette(palette);
    sproxy = new QGraphicsProxyWidget;

    sproxy = cscene->addWidget(widget);
    sproxy->setZValue(z + 1);
    pic[0]->setOpacity(0.0);
    pic[1]->setOpacity(0.0);
    this->setenabled(false);

    sproxy->setPos(CView->mapToScene(CView->viewport()->width() / 2 - 277,
                                     CView->viewport()->height() / 2 - 282));

    connect(close,SIGNAL(released()),this,SLOT(closesessionpage()));
}

/*
 * Creates Shortcuts
 */
void Interface::createShortCuts()
{
    Bsc = new QShortcut(QKeySequence(tr("F1")),this);
    Csc = new QShortcut(QKeySequence(tr("F2")),this);
}

/*
 * Sets Status Bar
 */
void Interface::setStatusBar()
{
    ulb = new QLabel;
    timelb = new QLabel;
    dq = new QLabel;
    mode = new QLabel;

    timelb->setAlignment(Qt::AlignCenter);
    mode->setAlignment(Qt::AlignCenter);
    dq->setAlignment(Qt::AlignRight);

    sBar->addWidget(ulb,1);
    sBar->addWidget(timelb,1);
    sBar->addWidget(mode,1);
    sBar->addWidget(dq,1);
}

/*
 * Sets the Status Bar Information
 */
void Interface::setSBText(QString user, bool c)
{
    ulb->setText("User Logged In : " + tr("<b>") + user + "<b>");
    timelb->setText("Login Time : " + tr("<b>") + QDateTime::currentDateTime().toString("dddd hh:mm:ss AP MMM yyyy") + "<b>");
    if(c)
        mode->setText("Mode : " + tr("<b>") + "Database" + tr("<b>"));
    else
        mode->setText("Mode : " + tr("<b>") + "Manual" + tr("<b>"));
    dq->setText("<b>DocmaQ</b> ");
}

/* void set_ct(Student *)
 * Called : to fill the student details with Line Items
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
       acyear->setText(student->acyear);

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
       acyear->setText(student->acyear);

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

/* set_tc(Student *)
 * Called :
 * Performs :
 */
void Interface::set_tc(Student *student)
{
    tc_sno->setPlainText(temp.sprintf("%05d",student->c_sno[0]));
    stname->setPlainText(student->name);
    fname->setPlainText(student->father_name);
    roll->setPlainText(student->roll);

    tc_type->setPlainText(student->std_ex->type);
    admno->setPlainText(student->roll);
    tcitems->at(0)->setPlainText(student->std_ex->dob);
    tcitems->at(1)->setPlainText(student->std_ex->doa);
    tcitems->at(2)->setPlainText(student->course);
    tcitems->at(3)->setPlainText(student->branch);
    tcitems->at(4)->setPlainText(student->std_ex->dol);
    tcitems->at(5)->setPlainText(student->std_ex->qualified);
    tcitems->at(6)->setPlainText(student->std_ex->dues);
    tcitems->at(7)->setPlainText(student->std_ex->disp_m);
    tcitems->at(8)->setPlainText(student->std_ex->conduct);
    tcitems->at(9)->setPlainText(student->std_ex->community);
    tcitems->at(10)->setPlainText(student->std_ex->remarks);
}

/* show_ct(int)
 * Called : By set_ct(), resetView()
 * Performs : Displays the appropriate Certificate
 */
void Interface::show_ct(int vpic)
{
    switch(vpic)
    {
    case 0: // Bonafide
        pic[0]->setOpacity(1.0);
        pic[1]->setOpacity(0.0);

        sno->setPos(168,131);
        cdate->setPos(515,129);
        stname->setPos(230,191);
        co->setPos(505,191);
        fname->setPos(115,216);
        roll->setPos(206,244);
        cdetails->setPos(273,271);

        break;
    case 1: // Conduct
        pic[0]->setOpacity(0.0);
        pic[1]->setOpacity(1.0);

        sno->setPos(148,52);
        cdate->setPos(520,56);
        stname->setPos(164,143);
        co->setPos(502,143);
        fname->setPos(57,175);
        roll->setPos(152,203);
        cdetails->setPos(227,233);
      }
}

/*
 * Resets View
 */
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

/*
 * Hides/Shows Details
 */
void Interface::showTc(bool var)
{
    pic[2]->setVisible(var);

    var = !var;

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

/*
 * Window Resize Event
 */
void Interface::viewresized()
{
    int w = CView->viewport()->width();
    int h = CView->viewport()->height();
    int mw = w/2;
    int x = mw - 107;

    up_stp->setPos(CView->mapToScene(CView->viewport()->pos()));
    up_stp->setRect(0,0,w,30);

    dn_stp->setPos(CView->mapToScene(0,h-30));
    dn_stp->setRect(0,0,w,30);

    // re position menu and session buttons
    menu->setPos(CView->mapToScene(CView->viewport()->pos()));
    session->setPos(CView->mapToScene(QPoint(w - 87,0)));

    // re position the arrow buttons
    /*larrow->setPos(CView->mapToScene(0,mh));
    rarrow->setPos(CView->mapToScene(w - 21,mh));
    uarrow->setPos(CView->mapToScene(mw - 145, h - 20));
    darrow->setPos(CView->mapToScene(mw + 5, h - 20));
    */

    larrow->setPos(CView->mapToScene(x,h-27));
    uarrow->setPos(CView->mapToScene(x + 55,h-27));
    darrow->setPos(CView->mapToScene(x + 110,h-27));
    rarrow->setPos(CView->mapToScene(x + 165,h-27));
    rollproxy->setPos(CView->mapToScene(QPoint(mw - 89,0)));

    // re postion panels
    QPointF pt,pt2;
    pt = CView->mapToScene(QPoint(-87,h - 275));
    s1->assignProperty(lproxy,"pos",pt);

    pt2 = CView->mapToScene(QPoint(w, h - 275));
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

    if(tpl)
        tpanel->setPos(CView->mapToScene(mw - 215, h - 50));
}

/*
 * Full Screen Mode
 */
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

/*
 * Displays Various Message Boxes
 */
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

/*
 * Closes Session Page
 */
void Interface::closesessionpage()
{
    cscene->removeItem(sproxy);
    this->setenabled(true);
    this->rollnoLe->setFocus();
}

/*
 * Enables View Navigations
 */
void Interface::setenabled(bool v)
{
    larrow->enable(v);
    rarrow->enable(v);
    uarrow->enable(v);
    darrow->enable(v);
    menu->enable(v);
    session->enable(v);
    rollboxGb->setEnabled(v);
    printButton->setEnabled(v);
}

/*
 * Shows About Box
 */
void Interface::about_docmaq()
{
    About_DocmaQ *abt = new About_DocmaQ(this);
    abt->move(this->width()/2 - 202, this->height()/2 - 218);
    abt->show();
    aboutb->setDown(false);
}

/*
 * Close Event
 */
void Interface::closeEvent(QCloseEvent *event)
{
   event->ignore();
   emit quit();
}
