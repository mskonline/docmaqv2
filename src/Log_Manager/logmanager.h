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

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QWidget>
#include <QCloseEvent>
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

signals:
    void lmclose();

private:
    QTableWidget *table;
    QTableWidgetItem *item;
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
    void enterRecord(const QString& line);
    void find();
    void findFiles(const QDir &directory, const QStringList &files,const QString &text);
    void searchType();
    void clearTable();
    void highlightItems();
    void unHighlightItems();
    void closeEvent(QCloseEvent *);
};
#endif
