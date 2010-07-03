//---------------AUTHOR:PRANEETH--------------------------------------------
//------------PROGRAM:AUTHENTICATION-MANAGER '&' DATABASE MANAGER-----------
#include <QtGui>
#include <QDebug>
#include "authendialog.h"



AuthenDialog::AuthenDialog(QDialog* parent) :QDialog(parent,Qt::WindowMaximizeButtonHint)
{

    setupUi(this);

    QCoreApplication::setAttribute ( Qt::AA_ImmediateWidgetCreation);

    settings= new QSettings( QSettings::SystemScope,"Qogency","DocmaQ");

    //---------------first page

    connect(passwordLE,SIGNAL(textChanged(const QString &)),this,SLOT(enableLoginButton(const QString &)));
    connect(passwordLE,SIGNAL(returnPressed()),loginButton,SLOT(animateClick()));


    connect(loginButton,SIGNAL(clicked()),this,SLOT(loginPageAuthentication()));


    // -----------to quit the application
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    userIDLE->setFocus();

    sb=NULL;
    timer=NULL;
}



void AuthenDialog:: enableLoginButton(const QString &password)
{
    loginButton->setEnabled(!(password.isEmpty()||userIDLE->text().isEmpty()));


}

void AuthenDialog::on_optionsList_currentRowChanged(int row)
{

    if(row==1 && tabWidget->tabText(tabWidget->currentIndex())=="DocmaQ")

    {
        timer=new QTimer(this);
        timer->setInterval(100);
        timer->start();
        connect(timer,SIGNAL(timeout()),this,SLOT(marquee()));

    }
    else if(timer)
    {
        timer->stop();
        if(sb)
            sb->setValue(0);
    }
}


void AuthenDialog::marquee()
{
    sb=textEdit->verticalScrollBar();
    sb->setValue(sb->value()+1);
    if(sb->value()==sb->maximum())
        sb->setValue(0);
}

void AuthenDialog::on_tabWidget_currentChanged(int row)
{

    if(tabWidget->tabText(row)=="DocmaQ")
    {
        sb->setValue(0);
        timer->start();
    }

}

void AuthenDialog::loginPageAuthentication()
{
    emit isAdmin(userIDLE->text());

}


void AuthenDialog::authenticate(const QString& passwd)
{

    if(passwordLE->text()==passwd)
    {
        done(QDialog::Accepted);
        emit userAuthenticated();
    }
    else
    {
        messageBox("Authentication Manager","UserID and Password do not match!!!  ");
        passwordLE->clear();
    }

}

inline void AuthenDialog::messageBox(char *title,char *message)
{
    QMessageBox::information(this,tr(title),tr(message),
                             QMessageBox::Ok|QMessageBox::Default,
                             QMessageBox::NoButton,QMessageBox::NoButton);

}

AuthenDialog()::~AuthenDialog()
{
}
