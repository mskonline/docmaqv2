/*  DocmaQ v2.1, Credential Publishing System
    Copyright (C) 2011 M.Sai Kumar <msk.mymails@gmail.com>
                                K.Praneeth <praneethk@in.com>
                                M.Kiran <mola.kiran@gmail.com>

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

#include "./App_Manager/appmanager.h"
#include "./App_Manager/thread.h"
#include <QtGui/QApplication>
#include <QSharedMemory>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedMemory sharedMemory("d_configure");

    if (sharedMemory.create(1) && sharedMemory.error() != QSharedMemory::AlreadyExists)
    {
        // Check if DocmaQ is already running
        QSharedMemory sm("docmaq");

        if(sm.create(1) && sm.error() != QSharedMemory::AlreadyExists)
        {
            sharedMemory.detach();

            // Load plugins
            a.addLibraryPath("./plugins");

            // Main app class initialisation
            AppManager *app = new AppManager();
            return a.exec();
        }
        else
        {
            QMessageBox::critical(0,"DocmaQ Error             ","Application is already running.");
            return 0;
        }
    }
    else
    {
        QMessageBox::critical(0,"DocmaQ Error            ", " DocmaQ Configure is running. Please Close it first to open DocmaQ." );
        return 0;
    }
}
