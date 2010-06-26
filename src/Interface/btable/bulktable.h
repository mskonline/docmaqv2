#ifndef BULKTABLE_H
#define BULKTABLE_H

#include <QTableWidget>
#include <QDebug>
#include <QKeyEvent>

class BulkTable : public QTableWidget
{
    Q_OBJECT

public:
    QFont ft;
    int rc,st_ptr;
    BulkTable(QWidget *parent = 0);
    void add_roll(QString,QString);
    void clearTable();

signals :
    void removeSt(int);

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // BULKTABLE_H
