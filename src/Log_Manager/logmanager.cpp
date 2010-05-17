#include <QtUiTools>
#include <QtGui>
#include <QDebug>
#include "logmanager.h"


Logmanager::Logmanager()
{

    settings= new QSettings( QSettings::SystemScope,"Quogency","DocmaQ");
    QWidget *formWidget = loadUiFile("./forms/logmanager.ui");
    searchButton = qFindChild<QPushButton*>(this, "searchButton");
    textEdit = qFindChild<QTextEdit*>(this, "textEdit");
    searchCombo = qFindChild<QComboBox*>(this, "searchCombo");
    checkBox = qFindChild<QCheckBox*>(this, "checkBox");
    tabWidget = qFindChild<QTabWidget*>(this, "tabWidget");
    tableWidget = qFindChild<QTableWidget*>(this, "tableWidget");
    dateEdit = qFindChild<QDateEdit*>(this, "dateEdit");
    goButton = qFindChild<QPushButton*>(this, "goButton");
    logDateLabel = qFindChild<QLabel*>(this, "logDateLabel");
    resultLabel = qFindChild<QLabel*>(this, "resultLabel");
    foundLabel = qFindChild<QLabel*>(this, "foundLabel");
    filesFoundLabel = qFindChild<QLabel*>(this, "filesFoundLabel");

    connect(dateEdit,SIGNAL(editingFinished ()),this,SLOT(on_goButton_clicked()));


    QMetaObject::connectSlotsByName(this);



    //arranging tableWidget
    tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    tableWidget->verticalHeader()->show();
    tableWidget->setShowGrid(true);

    //Setting up Default cursor

    QTextDocument *document = textEdit->document();
    defaultcursor=QTextCursor(document);
    textEdit->setTextCursor(defaultcursor);
    textEdit->ensureCursorVisible ();


    textEdit->setReadOnly(true);
    textEdit->setWordWrapMode(QTextOption::NoWrap);
    textEdit->setUndoRedoEnabled(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(formWidget);
    setLayout(layout);
    setWindowTitle(tr("Log Manager"));

    setGeometry(175,175,645,550);
    isFirstTime = true;

    // setting Log directory path
    /* if(settings->value("generalsettings/logmode","local")=="central")
    {
     path=settings->value("generalsettings/logpath","//10.4.9.57/srinivas/logs/").toString();

     if(!path.endsWith("\\"))
        path=path+"\\";
    }
     else
     */

    path="./logs/";

    directory=QDir(path);

    loadTextFile(QDate::currentDate());

    if (settings->status()!= QSettings::NoError)
    {
        messageBox("Log Path Error","Log Path Cannot be fetched Contact \"Software Dept\"  for help,");
    }

}

QWidget* Logmanager::loadUiFile(QString str)
{
    QUiLoader loader;

    QFile file(str);
    file.open(QFile::ReadOnly);

    QWidget *formWidget = loader.load(&file, this);
    file.close();

    return formWidget;
}

void Logmanager::loadTextFile(const QDate& date)
{

    QString filename = date.toString("dd.MMM.yyyy");

    dateEdit->setDate(date);

    QFile inputFile(path+filename+".Log");

    if(!inputFile.exists())
        inputFile.setFileName(path+filename+".Logc");

    logDateLabel->setText(filename);

    if (inputFile.open(QIODevice::ReadOnly))
    {
        textEdit->clear();
        QDataStream in(&inputFile);
        in.setDevice(&inputFile);

        QString line(" "), data("");

        while (!in.atEnd())
        {
            in>>line;

            if (!line.isEmpty())
                data+=line+"\n";
        }

        inputFile.close();
        textEdit->setPlainText(data);
    }

    else
    {
        textEdit->clear();
        textEdit->setPlainText("\n\n\n\t\t         Sorry, Log Unavailable !!");
    }
}


void Logmanager::on_searchCombo_editTextChanged()
{
    searchButton->setEnabled(true);
}

void Logmanager::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Enter:
    {
        searchButton->animateClick();

    }
    break;
    case Qt::Key_Backspace:
        searchCombo->clear();
        break;

    }
}

void Logmanager::on_searchButton_clicked()
{
    if (!checkBox->isChecked())
    {
        int result=0;
        textEdit->setTextCursor(defaultcursor);

        QString searchString = searchCombo->currentText();
        QTextDocument *document = textEdit->document();


        bool found = false;

        if (isFirstTime == false)
            document->undo();

        if (searchString == "")
        {
            messageBox("Empty Search Field","The search field is empty. Please enter a word and click Find.");
        }
        else
        {
            QTextCursor highlightCursor(document);
            QTextCursor cursor(document);

            cursor.beginEditBlock();

            QTextCharFormat plainFormat(highlightCursor.charFormat());
            QTextCharFormat colorFormat =plainFormat;

            colorFormat.setForeground(Qt::white);
            colorFormat.setBackground(QBrush(QColor(85,170,255)));
            colorFormat.setFont(QFont("Verdana",10,QFont::Black,true));

            textEdit->setTextCursor(highlightCursor);

            while (!highlightCursor.isNull() && !highlightCursor.atEnd())
            {
                highlightCursor = document->find(searchString, highlightCursor, QTextDocument::FindWholeWords);

                if (!highlightCursor.isNull())
                {
                    found = true;
                    result++;   //holds the no of results found

                    /*highlightCursor.setPosition(pos.at(0));
                    textEdit->setTextCursor(highlightCursor);
                    textEdit->ensureCursorVisible ();  */

                    highlightCursor.movePosition(QTextCursor::WordRight,
                                                 QTextCursor::KeepAnchor);
                    highlightCursor.mergeCharFormat(colorFormat);

                }

            }


            resultLabel->setText("Results Found : "+QString().setNum(result));

            cursor.endEditBlock();
            isFirstTime = false;

            if (found == false)
            {
                foundLabel->setText("Word not found.. !!");
            }
            else
            {
                foundLabel->clear();
                searchButton->setEnabled(false);
            }
        }
    }
    else
    {
        find();
    }

}

void Logmanager::on_goButton_clicked()
{

    textEdit->setTextCursor(defaultcursor);

    loadTextFile(dateEdit->date());

    tabWidget->setCurrentIndex(0);

    if (!(searchCombo->currentText()==""))
        on_searchButton_clicked();

}

void Logmanager::on_checkBox_toggled(bool flag)
{
    if (flag)
    {
        searchButton->setEnabled(true);
        tabWidget->setCurrentIndex(1);
    }
    else
    {
        tabWidget->setCurrentIndex(0);

    }

}

/*void Logmanager::on_optionsButton_toggled(bool flag)
{
     QWidget *formWidget = loadUiFile("../forms/optionswidget.ui");
     keepLabel1 = qFindChild<QLabel*>(this, "keepLabel1");
     keepLabel2 = qFindChild<QLabel*>(this, "keepLabel2");
     keepLe = qFindChild<QLineEdit*>(this, "keepLe");
     QVBoxLayout *layout = new QVBoxLayout;
     layout->addWidget(formWidget);
     setLayout(layout);
     formWidget->show();
     {
     }

     keepLabel1->setVisible(flag);
     keepLabel2->setVisible(flag);
     keepLe->setVisible(flag);

}*/

void Logmanager::find()
{
    tabWidget->setCurrentIndex(1);

    tableWidget->setRowCount(0);


    QString text = searchCombo->currentText();


    QStringList files,ext;
    ext<<"*.Log"<<"*.Logc";

    files << directory.entryList(ext,QDir::Files,QDir::Time);

    if (!text.isEmpty())
        files = findFiles(directory, files, text);
    showFiles(directory, files);
}

QStringList Logmanager::findFiles(const QDir &directory, const QStringList &files,
                                  const QString &text)
{
    QProgressDialog progressDialog(this);
    progressDialog.setCancelButtonText(tr("&Cancel"));
    progressDialog.setRange(0, files.size());
    progressDialog.setWindowTitle(tr("Finding Log Files...."));

    QStringList foundFiles;

    for (int i = 0; i < files.size(); ++i)
    {
        progressDialog.setValue(i);
        progressDialog.setLabelText(tr("Searching file number %1 of %2...")
                                    .arg(i).arg(files.size()));
        qApp->processEvents();

        if (progressDialog.wasCanceled())
            break;

        QFile logfile(directory.absoluteFilePath(files[i]));

        if (logfile.open(QIODevice::ReadOnly))
        {
            QString line;
            QDataStream in(&logfile);

            while (!in.atEnd())
            {
                if (progressDialog.wasCanceled())
                    break;
                in>>line;
                if (line.contains(text)||line.contains(text.toUpper()))
                {
                    foundFiles << files[i];
                    break;
                }
            }
        }
    }
    return foundFiles;
}

void Logmanager::showFiles(const QDir &directory, const QStringList &files)
{
    for (int i = 0; i < files.size(); ++i)
    {
        QFile file(directory.absoluteFilePath(files[i]));
        qint64 size = QFileInfo(file).size();

        QTableWidgetItem *fileNameItem = new QTableWidgetItem(files[i]);
        fileNameItem->setFlags(Qt::ItemIsEnabled);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(tr("%1 KB")
                .arg(int((size + 1023) / 1024)));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setFlags(Qt::ItemIsEnabled);

        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, fileNameItem);
        tableWidget->setItem(row, 1, sizeItem);
    }
    filesFoundLabel->setText(tr("%1 Log file(s) found").arg(files.size()));
}

void Logmanager::on_tableWidget_cellDoubleClicked( int row, int column )
{
    QString str=tableWidget->item (  row,  column )->text();
    if(str.endsWith('c'))
        str.chop(5);
    else
        str.chop(4);
    tabWidget->setCurrentIndex(0);
    loadTextFile(QDate::fromString(str,"dd.MMM.yyyy"));

}

void Logmanager::on_tabWidget_currentChanged(int index)
{
    textEdit->setTextCursor(defaultcursor);
    searchButton->setEnabled(true);

    if (index == 1)
        checkBox->setChecked(Qt::Checked);
    else
        checkBox->setChecked(Qt::Unchecked);

}



