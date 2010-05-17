#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>

class mythread:public QThread
{
    public:
    static void msleep(int i)
    {
        sleep(i);
    }
};
#endif // MYTHREAD_H
