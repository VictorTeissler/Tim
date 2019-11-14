#include "mainwindow.h"
#include "mythread.h"
#include <QApplication>
#include <QtCore>
#include <QThread>
#include <QMutex>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setsid();
    MainWindow w;
    MyThread t;
    t.w = &w;
    t.start();
    QObject::connect(&t, SIGNAL(finished()), &a, SLOT(quit()));
    QObject::connect(&t, SIGNAL(signalGUI()), &w, SLOT(refresh()));
    w.show();
    return a.exec();
}
