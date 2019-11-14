#include "mythread.h"
#include <QApplication>
#include <QtCore>
#include <QThread>
#include <QMutex>
#include <QImage>
#include <QPixmap>


void MyThread::run()
{
    QRgb value;
    int shmid;
    key_t key;
    char *shm;

    key = 0x414141414101;

    //qDebug("Logo display");
    //qDebug("Setting up SHM");

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget QT Side");
        return;
    }

    if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        return;
    }

    bzero((void *)shm, SHMSZ);

    // wait to be set to zero
    *(shm+SHMSZ-1) = 3;
    while ((char *)*(shm+SHMSZ-1) == (char *)3) {QTimer();}
    QThread::usleep(1000000);

    while (1)
    {
        while ((char *)*(shm+SHMSZ-1) == (char *)0) {QThread::usleep(15385);} // wait until data is in the buffer
        if ((char *)*(shm+SHMSZ-1) == (char *)2)
        {
            //qDebug("We see the EXIT!");
            return;
        }
        *(shm+SHMSZ-1) = 0;
        //w->vga = QImage(800, 600, QImage::Format_RGB32);
        int x,y;
        //w->vga.loadFromData()
        //w->vga.loadFromData(shm, SHMSZ-2, QImage::Format_ARGB32)
        for (y=0;y<600;y++)
        {
            for (x=0;x<800;x++)
            {
                value = qRgb(shm[x*4+y*800*4], shm[x*4+y*800*4+1], shm[x*4+y*800*4+2]);
                w->vga.setPixel(x,y,value);
                /*if (shm[x*4+y*800*4] || shm[x*4+y*800*4+1] || shm[x*4+y*800*4+2]) {
                    qDebug("set x:%d, y:%d (r:0x%x,g:0x%x,b:0x%x)\n", x, y, shm[x*4+y*800*4], shm[x*4+y*800*4+1], shm[x*4+y*800*4+2]);
                }*/
            }
        }
        emit signalGUI();
        //qDebug("Refreshed Screen!");
    };
}
