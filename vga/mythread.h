#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QMutex>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <strings.h>
#include "mainwindow.h"

#define SHMSZ (800*600+1)*4 //1 for the control byte at the end

class MyThread : public QThread {
    Q_OBJECT

  protected:
    virtual void run();
  public:
    MainWindow* w;
  signals:
    void signalGUI();
};

#endif // MYTHREAD_H
