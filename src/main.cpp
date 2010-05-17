#include <QtGui/QApplication>
#include "./App_Manager/appmanager.h"
#include "./App_Manager/thread.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    AppManager *app = new AppManager();
//    myThread::delay(2);

    // Load DB , Settings Module Authentication Manager
    app->load_final_modules();



    return a.exec();
}