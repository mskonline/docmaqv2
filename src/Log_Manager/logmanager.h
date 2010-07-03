#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QWidget>
#include <QDir>
#include "ui_logmanager.h"


class Logmanager : public QWidget,public Ui::Logmanager
{
    Q_OBJECT

public:
    Logmanager(QWidget *parent=0);
    ~Logmanager();


private slots:

    void on_viewLogButton_clicked();
    void prepareSearch();
    void currentChanged(int index);
    void searchAll();
    void searchCurrent();


private:

    QTableWidget *table;
    QLabel *label;
    QAction *searchCurrentAction,*searchAllAction;
    QString path;
    QDir directory;
    QList<QTableWidgetItem*> items;
    QString cdate,sdate,//certificate and log date b/w tab changes
    prevText;//to store previous search text

    QString cfound,sfound;//certificate and log count b/w tab changes
    int nitems;
    int currentAction;//specifies whether search current/search all mode

    void loadTextFile(const QDate& date);


    // multiple log search
    void enterRecord(const QString& line);
    void find();
    void findFiles(const QDir &directory, const QStringList &files,const QString &text);

    void searchType();
    void clearTable();
    void highlightItems();
    void unHighlightItems();

};
#endif
