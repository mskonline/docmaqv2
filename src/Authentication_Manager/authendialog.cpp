/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 K.Praneeth <praneethk@in.com>

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

#include "authendialog.h"
#include <QtGui>

/*
 * Constructor
 */
AuthenDialog::AuthenDialog(bool mode,bool con, QString dbstatus,QDialog* parent) :QDialog(parent,Qt::WindowMaximizeButtonHint)
{
    // Setup Interface
    setupUi(this);
    fig1->setPixmap(QPixmap(QString::fromUtf8("./images/docmaq.png")));

    // Check mode. mode is passed on from App Manager
    if(mode)
    {
        if(con)
        {
            connectionLabel->setText(dbstatus);
            details->hide();
        }
        else
            connectionLabel->setText("    Connection with Database failed");
    }
    else
    {
        connectionLabel->setText(dbstatus);
        details->hide();
    }

    if(!mode)
    {
        comboBox->setItemText(0,"Staff");
        comboBox->setCurrentIndex(0);
        comboBox->setEditable(false);
    }
    comboBox->setCompleter(0);
    comboBox->setFocus();

    this->dbstatus = dbstatus;
    this->mode = mode;

    // Setup the About Animation
    QGraphicsScene *scene =  new QGraphicsScene;
    pix = new Pixmap(QPixmap("./images/intro.png"));

    scene->addItem(pix);
    cview->setScene(scene);

    pix->setPos(0,0);
    anim = new QPropertyAnimation(pix,"pos");
    anim->setStartValue(QPointF(0,0));
    anim->setEndValue(QPointF(0,-939));
    anim->setEasingCurve(QEasingCurve::Linear);
    anim->setDuration(50000);
    anim->setLoopCount(1);

    QCoreApplication::setAttribute ( Qt::AA_ImmediateWidgetCreation);
    // QSettings Object
    settings= new QSettings( QSettings::SystemScope,"DocmaQ","App");

    // Connections
    connect(passwordLE,SIGNAL(returnPressed()),loginButton,SLOT(animateClick()));
    connect(details,SIGNAL(released()),this,SLOT(err_msg()));
    connect(loginButton,SIGNAL(released()),this,SLOT(loginPageAuthentication()));
    connect(anim,SIGNAL(finished()),this,SLOT(anim_stopped()));

    // Set Focus to Username Combo Box
    comboBox->setFocus();
}

/* on_optionsList_currentRowChanged(int)
 * Called : When User selects the options : Login
 * or About
 * Performs : Starts or Stops the About Animation
 */
void AuthenDialog::on_optionsList_currentRowChanged(int row)
{
    if(row)
    {
       // about page
       QTimer::singleShot(7000,anim,SLOT(start()));
    }
    else
    {
       // login page
        anim->stop();
        pix->setPos(0,0);
    }
}

/* anim_stopped()
 * Called : When Animation ends i.e. when the About
 * Image reaches the End
 * Performs : Restarts the Animation
 */
void AuthenDialog::anim_stopped()
{
    if(optionsList->currentRow() == 0)
        return;
    else
        QTimer::singleShot(10000,this,SLOT(reset_anim()));
}

/* reset_anim()
 * Called : by anim_stopped()
 * Performs : Resets the Image and restarts the animation
 */
void AuthenDialog::reset_anim()
{
    pix->setPos(0,0);
    on_optionsList_currentRowChanged(1);
}

/* err_msg()
 * Called : when User clicks Details Button
 * Performs : Shows the Error Message (occured while connecting to DB)
 */
void AuthenDialog::err_msg()
{
    QMessageBox::information(this,"Database Connection Error", "Attempt to Connect to Database failed.\n\n MySQL Reports :" + dbstatus +
                                        "\n\n You can still Login in Manual Mode");
}

/* loginPageAuthentication()
 * Called : When User Clicks Login Button
 * Performs : Authenticates the User
 */
void AuthenDialog::loginPageAuthentication()
{
    if(comboBox->currentText().isEmpty() or passwordLE->text().isEmpty() )
        return;

    // To enable admin authentication irrespective of mode
    if(comboBox->currentText()=="Admin")
        authenticate1("admin");
    else if(mode)
        emit checkuser(comboBox->currentText());
    // To enable staff authentication incase if DB is not available
    else if(comboBox->currentText() == "Staff")
        authenticate1("staff");
    else {}
}

/* authenticate1(QString)
 * Called : by loginPageAuthentication()
 * Performs : Checks whether the entered Password is matching
 * with the one stored in Windows Registry
 */
void AuthenDialog::authenticate1(QString name)
{
    QCryptographicHash hashed(QCryptographicHash::Sha1);
    hashed.addData (QVariant(passwordLE->text()).toByteArray());

    if(settings->value("logins/"+name).toByteArray()==hashed.result())
    {
        int i = 0; // Staff
        done(QDialog::Accepted);
        if(comboBox->currentText() == "Admin")
            i = 1; // Admin

        emit userAuthenticated(i);
    }
    else
    {
        messageBox("DocmaQ Authentication","Invalid Password. Please try again.");
        passwordLE->clear();
    }
}

void AuthenDialog::authenticate(const QString& passwd)
{
    if(passwordLE->text()==passwd)
    {
        int i = -1; // Legitimate User
        done(QDialog::Accepted);
        emit userAuthenticated(i);
    }
    else
    {
        messageBox("DocmaQ Authentication","Invalid Password. Please try again.");
        passwordLE->clear();
    }
}

/*
 * Message Box
 */
inline void AuthenDialog::messageBox(QString title,QString message)
{
    QMessageBox::information(this,title,message,
                             QMessageBox::Ok|QMessageBox::Default,
                             QMessageBox::NoButton,QMessageBox::NoButton);
}

/*
 * CloseEvent
 */
void AuthenDialog::closeEvent(QCloseEvent *e)
{
    qApp->quit();
}

/*
 * Destructor
 */
AuthenDialog::~AuthenDialog()
{
}
