#ifndef LOGS_H
#define LOGS_H

#include <QTextEdit>
#include <QFile>
#include <QDataStream>


class Logmanager;
class QSettings;

class logs:public QWidget
{
    Q_OBJECT
private :
    QTextEdit *lgs;
    QFile file;
    QDataStream out;
    QString dt,path;
    QSettings *settings;
    void checkLogDirectory();
    void removeExcessLogFiles();
    bool createServerFile(const QString& filename);
    QFile serverfile;
    enum certificatetype{BONAFIDE,CONDUCT,TC};
    QStringList snorollno;
public :
    Logmanager *lm;
    logs(QTextEdit *);

    void write(const QString & str);
    void setCount(int count2day,int totalcount,int type=0);
    QList <int> getCount(int type=0);
    QStringList getLastSnoRollNo(int type=0);
    void setLastSnoRollNo(int serialno,QString rollno,int type=0);    
    void setCurrentYear(int from,int to);
    QStringList getCurrentYear();

public slots:

    void save();
    void createlogmanager();


};

#endif
