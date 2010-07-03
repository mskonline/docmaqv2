#ifndef LOGS_H
#define LOGS_H

#include <QFile>
#include <QTextStream>
#include<QCloseEvent>

class Logmanager;
class QSettings;
class GetSettings;

class log:public QObject
{
    Q_OBJECT
private :

    GetSettings *getSettings;
    QSettings *settings;
    QFile file;
    QTextStream out,out1;
    QString dt,path;//path=current path
    QString username;


    QFile sessionFile;

     void closeEvent(QCloseEvent *e);
signals:
    void backupSignal(int);


public :
    Logmanager *lm;
    log();
    ~log();

    void writeCertificateLog(const QString& rollno,const short& type);

    void startSession(const QString& userid,const QString& username);
    void endSession();

public slots:

    void createlogmanager();



};

#endif
