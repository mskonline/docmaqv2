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

#include "logmanager.h"
#include <QMenu>
#include <QProgressDialog>
#include <QSettings>
#include <QTextStream>

Logmanager::Logmanager(QWidget *parent): QWidget(parent,Qt::Window)
{
    setupUi(this);

    searchCurrentAction=new QAction(" in current log ",this);
    searchAllAction=new QAction(" in all logs ",this);

    QMenu *searchMenu= new QMenu(this);
    searchMenu->addAction(searchCurrentAction);
    searchMenu->addAction(searchAllAction);
    searchButton->setMenu(searchMenu);

    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentChanged(int)));
    connect(searchLE,SIGNAL(textEdited(QString)),this,SLOT(prepareSearch()));
    connect(searchLE,SIGNAL(returnPressed()),this,SLOT(prepareSearch()));
    connect(searchCurrentAction,SIGNAL(triggered()),this,SLOT(searchCurrent()));
    connect(searchAllAction,SIGNAL(triggered()),this,SLOT(searchAll()));
    QMetaObject::connectSlotsByName(this);

    certificateTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    sessionTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    QDate date=QDate::currentDate();

    sdate=" Log : "+date.toString("dd.MMM.yyyy")+" ";// to enable first time change of tabwidget

    path = "./logs/session/";
    table=sessionTable;
    label=logFlagLabel2;
    loadTextFile(date);//load session log,same as sdate for first time
    sfound=" Records Found: "+QString().setNum(table->rowCount())+" ";

    path="./logs/certificate/";
    directory=QDir(path);
    table=certificateTable;
    label=logFlagLabel;
    loadTextFile(date);//load cert log

    cfound=" Records Found: "+QString().setNum(table->rowCount())+" ";

    recordsLabel->setText(cfound);

    label=logFlagLabel;
    table=certificateTable;//reverting to initial table

    searchGB->hide();
    searchMsgLabel->hide();

    QSettings settings(QSettings::SystemScope,"Qogency","DocmaQ");

    settings.beginGroup("certificate");

    nPrintsLabel1->setText("Total:"+settings.value("total").toString()+"     B: "+settings.value("bonafide/count/total",0).toString()+"   C: "+settings.value("conduct/count/total",0).toString()+"   T: "+settings.value("tc/count/total",0).toString());
    nPrintsLabel2->setText("Total(today): "+settings.value("totaltoday",0).toString()+"     B: "+settings.value("bonafide/count/today",0).toString()+"   C:"+settings.value("conduct/count/today",0).toString()+"   T: "+settings.value("tc/count/today",0).toString());
    nPrintsLabel3->setText("Total(session): "+settings.value("sessiontotal",0).toString()+"     B: "+settings.value("bonafide/count/session",0).toString()+"   C:"+settings.value("conduct/count/session",0).toString()+"   T: "+settings.value("tc/count/session",0).toString());
    settings.endGroup();
    currentAction=0;
}

void Logmanager::on_viewLogButton_clicked()
{
    table->setAlternatingRowColors(true);
    loadTextFile(dateEdit->date());
    recordsLabel->setText(" Records Found: "+QString().setNum(table->rowCount())+" ");  
}

void Logmanager::loadTextFile(const QDate& date)
{
    label->hide();
    table->show();

    QString filename = date.toString("dd.MMM.yyyy");

    dateEdit->setDate(date);

    modeLabel->setText(" Log : "+date.toString("dd.MMM.yyyy")+" ");

    QFile inputFile(path+filename+".Log");

    int recordsFound=0;
    if (inputFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        table->setRowCount(0);
        QTextStream in(&inputFile);

        QString line;

        while (!in.atEnd())
        {
            line=in.readLine();

            if (!line.isEmpty())
            {
                recordsFound++;
                enterRecord(line);
            }
        }
        inputFile.close();
    }
    else
    {
        label->setText("No Log Recorded found on " + filename);
        table->hide();
        clearTable();
    }

    if(table->objectName().startsWith("s")&&date==QDate::currentDate())
    {
        QSettings settings(QSettings::SystemScope,"Qogency","DocmaQ");
        table->item((table->rowCount()-1),4)->setText("Logged in now");
        table->item((table->rowCount()-1),5)->setText(settings.value("certificate/sessiontotal",-1).toString());
    }
}

void Logmanager::clearTable()
{
    if(table)
    {
        int rowcount=table->rowCount();
        for(int i=0;i<rowcount;i++)
            table->removeRow(i);
        table->removeRow(0);
    }
}

void Logmanager::currentChanged(int index)
{
    if(index==0)
    {
        label=logFlagLabel;
        table=certificateTable;
        path="./logs/certificate/";

        sdate=modeLabel->text();
        modeLabel->setText(cdate);

        sfound=recordsLabel->text();
        recordsLabel->setText(cfound);
    }
    else
    {
        label=logFlagLabel2;
        table=sessionTable;
        path="./logs/session/";

        cdate=modeLabel->text();
        modeLabel->setText(sdate);

        cfound=recordsLabel->text();
        recordsLabel->setText(sfound);
    }
}

void Logmanager::prepareSearch()
{
    unHighlightItems();

    if(searchLE->text()!="")
    {
        if(currentAction==0)
            searchCurrent();
        else
            searchAll();
    }
    else
    {
        if(currentAction==0)
            searchTip->setText("Type text to search");
        else
            searchTip->setText("Type text and press\n    Enter to search");
    }
}

void Logmanager::searchCurrent()
{
    searchTip->setText("Type text to search");
    //connection fails if already connection exists --> Qt::UniqueConnection
    connect(searchLE,SIGNAL(textEdited(QString)),this,SLOT(prepareSearch()),Qt::UniqueConnection);
    searchButton->setText("  in current log ");
    currentAction=0;
    if(table->isHidden()&&!searchLE->text().isEmpty())
    {
        searchMsgLabel->show();
        searchTip->setText("");
    }
    else
    {
        modeLabel->setText(" Search Results: Search Current");

        if(searchLE->text()!="")
        {
            table->setAlternatingRowColors(false);
            highlightItems();
        }
        else
            table->setAlternatingRowColors(true);
    }
    searchLE->setFocus();
}

void Logmanager::highlightItems()
{
    items=table->findItems(searchLE->text(),Qt::MatchContains);
    nitems=items.size();

    if(nitems==0)
        searchMsgLabel->show();
    else
        searchMsgLabel->hide();

    recordsLabel->setText(" Matches Found: "+QString().setNum(nitems)+" ");

    for(int i=0;i<nitems;i++)
    {
        items[i]->setBackground(Qt::darkBlue);
        items[i]->setFont(QFont ("Verdana", 8, QFont::Bold));
        items[i]->setForeground(Qt::white);
    }
}

void Logmanager::unHighlightItems()
{
    nitems=items.size();
    for(int i=0;i<nitems;i++)
    {
        items[i]->setBackground(Qt::white);
        items[i]->setFont(QFont ("MS Shell Dlg 2", 8, QFont::Normal));
        items[i]->setForeground(Qt::black);
    }
}

void Logmanager::searchAll()
{
    searchTip->setText("Type text and press\n    Enter to search");
    disconnect(searchLE,SIGNAL(textEdited(QString)),this,SLOT(prepareSearch()));
    table->show();
    label->hide();
    currentAction=1;
    modeLabel->setText(" Search Results: Search All");
    searchButton->setText(" in all logs ");

    if(prevText!=searchLE->text())
    {
        unHighlightItems();

        prevText=searchLE->text();
        if(prevText!="")
        {
            table->setAlternatingRowColors(false);
            find();
        }
    }
    if(prevText!="")
        highlightItems();
    searchLE->setFocus();
}

void Logmanager::find()
{
    table->setRowCount(0);

    QString text = searchLE->text();

    QStringList files,ext;
    ext<<"*.Log";

    directory.setPath(path);

    files << directory.entryList(ext,QDir::Files,QDir::Time);

    int nfiles;
    if( !searchLE->text().isEmpty()&&(nfiles=files.size())==0)
    {
        searchTip->setText("");
        searchMsgLabel->show();
    }
    else
        searchMsgLabel->hide();

    if (!text.isEmpty()&&nfiles!=0)
        findFiles(directory, files, text);
}

void Logmanager::findFiles(const QDir &directory, const QStringList &files,
                           const QString &text)
{
    //table->setSortingEnabled(false);
    QProgressDialog progressDialog(this);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, files.size());
    progressDialog.setWindowTitle(tr("Finding Log Files..."));
    progressDialog.show();

    int recordsFound=0;

    for (int i = 0; i < files.size(); ++i)
    {
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file %1 of %2...")
                                    .arg(i).arg(files.size()));
        qApp->processEvents();

        if (progressDialog.wasCanceled())
            break;

        QFile logfile(directory.absoluteFilePath(files[i]));

        if (logfile.open(QIODevice::ReadOnly|QIODevice::Text));
        {
            QString line;
            QTextStream in(&logfile);

            while (!in.atEnd())
            {
                if (progressDialog.wasCanceled())
                    break;

                line=in.readLine();

                if (line.contains(text)||line.contains(text,Qt::CaseInsensitive))
                {
                    enterRecord(line);
                    recordsFound++;
                }
            }
        }
    }

    progressDialog.close();
    recordsLabel->setText(" Matches Found: "+QString().setNum(recordsFound)+" ");
   // table->setSortingEnabled(true);
}

void Logmanager::enterRecord(const QString &line)
{
    QStringList recordField;

    recordField=line.split("~");

    int row = table->rowCount();

    table->insertRow(row);

    int columnCount=recordField.size();//column count varies as per words in a single record

    for(int i=0;i<columnCount;i++)
    {
        item = new QTableWidgetItem(recordField[i]);
        table->setItem(row,i, item);
        item->setTextAlignment(Qt::AlignCenter);
    }

    recordField.clear();
}

void Logmanager::closeEvent(QCloseEvent *e)
{
    emit lmclose();
    e->accept();
}

Logmanager::~Logmanager()
{}
