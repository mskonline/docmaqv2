#ifndef BULKTABLE_H
#define BULKTABLE_H

#include <QTableWidget>

class BulkTable : public QTableWidget
{
    Q_OBJECT
public:
    BulkTable(QWidget *parent = 0);
    int rc,st_ptr;
    QFont ft;
    void add_roll(QString,QString);
    void clearTable();
signals :
    void removeSt(int);
private:
    void KeyPressEvent(QKeyEvent *);
};

#endif // BULKTABLE_H
