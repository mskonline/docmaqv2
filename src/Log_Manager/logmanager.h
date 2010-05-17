#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QWidget>
#include <QList>
#include <QTextCursor>
#include <QMessageBox>
#include <QDir>

QT_BEGIN_NAMESPACE

class QPushButton;
class QTextEdit;
class QComboBox;
class QCheckBox;
class QTabWidget;
class QTableWidget;
class QDateEdit;
class QDate;
class QLabel;
class QSettings;
class QLineEdit;

QT_END_NAMESPACE


class Logmanager : public QWidget
{
    Q_OBJECT
public:
    Logmanager();
    void messageBox(char *title,char *message)
    {
        QMessageBox::information(this,tr(title),tr(message),
                                 QMessageBox::Ok|QMessageBox::Default,
                                 QMessageBox::NoButton,QMessageBox::NoButton);

    }
    // multiple log search
    void find();
    QStringList findFiles(const QDir &directory, const QStringList &files,const QString &text);
    void showFiles(const QDir &directory, const QStringList &files);
    void loadTextFile(const QDate& date);
    void keyPressEvent(QKeyEvent *e);


private slots:

    void on_searchButton_clicked();
    void on_goButton_clicked();
    void on_checkBox_toggled(bool);
    void on_searchCombo_editTextChanged();
    void on_tableWidget_cellDoubleClicked( int row, int column );
    void on_tabWidget_currentChanged(int index);

private:

    QWidget* loadUiFile( QString);
    void checkLogDirectory();
    QSettings *settings;
    QPushButton *searchButton,*goButton;
    QTextEdit *textEdit;
    QCheckBox *checkBox;
    QComboBox *searchCombo;
    QTabWidget *tabWidget;
    QTableWidget *tableWidget;
    QDateEdit *dateEdit;
    QLabel *logDateLabel,*resultLabel,*foundLabel,*filesFoundLabel;
    bool isFirstTime;
    QList<int> pos;
    QTextCursor defaultcursor;
    QString path;
    QDir directory;
    QStringList filter;

};
#endif
