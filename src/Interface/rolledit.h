/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 M.Sai Kumar <msk.mymails@gmail.com>

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
        QLineEdit::focusInEvent(e);
        emit rollfocus();
    }
};

#endif // ROLLLE_H
