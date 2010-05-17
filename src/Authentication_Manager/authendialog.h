#ifndef AUTHENWIDGET_H_INCLUDED
#define AUTHENWIDGET_H_INCLUDED

#include "ui_authendialog.h"


class QSettings;
class QDialog ;

class AuthenDialog:public QDialog,public Ui::AuthenDialog

{
    Q_OBJECT


public:

    AuthenDialog(QDialog* parent=0);

public slots:

    void authenticate(const QString &passwd);

private slots:

    void enableLoginButton(const QString &password);
    void loginPageAuthentication();
    void on_optionsList_currentRowChanged(int row);
    void on_tabWidget_currentChanged(int index);
    void marquee();
    // void user(QString userID);

private:
    QSettings *settings;
    QTimer *timer;
    QScrollBar *sb;
    void messageBox(char *title,char *message);

signals:
    void isAdmin(QString userID);
    void userAuthenticated();


};

#endif // AUTHENWIDGET_H_INCLUDED
