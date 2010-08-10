/* DocmaQ v2.0, Credential Publishing System
    Copyright (C) 2010 M.Sai Kumar <msk.mymails@gmail.com>

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

#include "about.h"
#include <QString>
#include <QFile>
#include <QFont>

About_DocmaQ::About_DocmaQ(QWidget *parent) : QWidget(parent, Qt::Window)
{
    setupUi(this);
    this->setWindowFlags(Qt::Window
                         | Qt::WindowCloseButtonHint);
    QWidget::setAttribute(Qt::WA_DeleteOnClose);
    QString license_file = "gpl.html";
    if (QFile::exists(license_file))
    {
        QFont fixed_font;
        fixed_font.setStyleHint(QFont::TypeWriter);
        fixed_font.setFamily("Courier");
        license->setFont(fixed_font);

        QFile f(license_file);
        if (f.open(QIODevice::ReadOnly))
            license->setText(QString::fromUtf8(f.readAll().constData()));

        f.close();
    }
    else
    {
        license->setText(
		"<i>" +
		tr("This program is free software; you can redistribute it and/or modify "
                   "it under the terms of the GNU General Public License as published by "
                   "the Free Software Foundation; either version 2 of the License, or "
                   "(at your option) any later version.") + "</i>");
    }

    authors->setText("<b>" + tr("Developers :") + tr("</b>") + "<br>" +
                     tr("M.Sai Kumar, CSE") + "<br>" + tr("Batch : 2006-10") + "<br>" +
                     tr("email : msk.mymails@gmail.com") + "<br><br>" +
                     tr("K.Praneeth, CSE") + "<br>" + tr("Batch : 2006-10") + "<br>" +
                     tr("email : praneethk@in.com") + "<br><br>" +
                     tr("M.Kiran, CSE") + "<br>" + tr("Batch : 2006-10") + "<br>" +
                     tr("email : mola.kiran@gmail.com"));

}

