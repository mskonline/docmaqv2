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

#ifndef AUTHENWIDGET_H_INCLUDED
#define AUTHENWIDGET_H_INCLUDED

#include "ui_authendialog.h"
#include <QCryptographicHash>
#include <QPropertyAnimation>

class QSettings;
class QDialog;

class AuthenDialog:public QDialog,public Ui::AuthenDialog
{
    Q_OBJECT

public:
    AuthenDialog(bool ,bool,QString,QDialog* parent=0);
    ~AuthenDialog();

public slots:
   void authenticate(const QString&passwd);

private slots:
    void loginPageAuthentication();
    void on_optionsList_currentRowChanged(int row);
    void err_msg();
    void reset_anim();
    void anim_stopped();

private:
    QString dbstatus;
    QSettings *settings;
    QPropertyAnimation *anim;
    Pixmap *pix;
    bool mode;
    void messageBox(QString title, QString message);
    void authenticate1(QString username);

protected :
    void closeEvent(QCloseEvent *);

signals:
    void checkuser(QString userID);
    void userAuthenticated(int);
};

#endif // AUTHENWIDGET_H_INCLUDED
