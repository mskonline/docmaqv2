#ifndef ROLLLE_H
#define ROLLLE_H

#include <QtGui>

class RollLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    RollLineEdit(QWidget *parent):QLineEdit(parent)
    {}
signals :
     void rollfocus();
private:
    void focusInEvent ( QFocusEvent * e )
    {
        emit rollfocus();
    }
};

#endif // ROLLLE_H
