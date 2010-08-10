/* DocmaQ Configure, Configuration Interface for DocmaQ v2.0
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

#include <QtGui/QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include "getconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory sharedMemory("d_configure");
    if (sharedMemory.create(1) && sharedMemory.error() != QSharedMemory::AlreadyExists)
    {
        a.addLibraryPath("./plugins");
        GetConfig get;
        return a.exec();
    }
    else
    {
        QMessageBox::critical(0,"Docmaq Configure Error  ","Domaq Configure is already running.");
        return 0;
    }    
}
