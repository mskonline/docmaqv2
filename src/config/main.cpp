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

    QSharedMemory sharedMemory("docmaq");

    if (sharedMemory.create(1) && sharedMemory.error() != QSharedMemory::AlreadyExists)
    {
        // Check if Configure is already running
        QSharedMemory sm("d_configure");

        if(sm.create(1) && sm.error() != QSharedMemory::AlreadyExists)
        {
            sharedMemory.detach();

            a.addLibraryPath("./plugins");
            GetConfig get;
            return a.exec();
        }
        else
        {
            QMessageBox::critical(0,"DocmaQ Configure Error", " DocmaQ Configure is already running." );
            return 0;
        }
    }
    else
    {
        QMessageBox::critical(0,"DocmaQ Configure Error  ","DocmaQ is running. Please Close it first to open DocmaQ Configure.");
        return 0;
    }    
}
