/* DocmaQ v2.0, Credential Publishing System
    Copyright (C) 2010 K.Praneeth <praneethk@in.com>

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

AuthenDialog::AuthenDialog(bool mode,bool con, QString dbstatus,QDialog* parent) :QDialog(parent,Qt::WindowMaximizeButtonHint)
{
    setupUi(this);
    fig1->setPixmap(QPixmap(QString::fromUtf8("./images/docmaq.png")));

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
    settings= new QSettings( QSettings::SystemScope,"DocmaQ","App");

    //---------------first page
    connect(passwordLE,SIGNAL(returnPressed()),loginButton,SLOT(animateClick()));
    connect(details,SIGNAL(released()),this,SLOT(err_msg()));
    connect(loginButton,SIGNAL(released()),this,SLOT(loginPageAuthentication()));
    connect(anim,SIGNAL(finished()),this,SLOT(anim_stopped()));
}

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

void AuthenDialog::anim_stopped()
{
    if(optionsList->currentRow() == 0)
        return;
    else
        QTimer::singleShot(10000,this,SLOT(reset_anim()));
}

void AuthenDialog::reset_anim()
{
    pix->setPos(0,0);
    on_optionsList_currentRowChanged(1);
}

void AuthenDialog::err_msg()
{
    QMessageBox::information(this,"Database Connection Error", "Attempt to Connect to Database failed.\n\n MySQL Reports :" + dbstatus +
                                        "\n\n You can still Login in Manual Mode");
}

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

inline void AuthenDialog::messageBox(QString title,QString message)
{
    QMessageBox::information(this,title,message,
                             QMessageBox::Ok|QMessageBox::Default,
                             QMessageBox::NoButton,QMessageBox::NoButton);
}

void AuthenDialog::closeEvent(QCloseEvent *e)
{
    qApp->quit();
}

AuthenDialog::~AuthenDialog()
{
}
