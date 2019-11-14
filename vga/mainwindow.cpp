#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QKeyEvent>
#include <QtDebug>



void MainWindow::keyPressEvent(QKeyEvent* ke)
{
    // try and filter for shift?
        char c;
        QMainWindow::keyPressEvent(ke);

        int index;
        for (index=0;index < KEYCACHESZ-1;index++)
        {
            if (shm[index] == 0)
            {
                c = ke->key() & 0xff;
                *(shm+index) = c;
                break;
            }
        }
}

void MainWindow::keyReleaseEvent(QKeyEvent* ke)
{
        //qDebug(Q_FUNC_INFO);
        //qDebug() << "mw" << ke->key() << "up";
        QMainWindow::keyPressEvent(ke); // aka the base class implementation
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    int shmid;
    key_t key;

    key = 0x414141414102;

    //qDebug("Setting up KB SHM...");

    if ((shmid = shmget(key, KEYCACHESZ, IPC_CREAT | 0666)) < 0)
    {
        perror("shmget");
        return;
    }

    if ((shm = (char *)shmat(shmid, NULL, 0)) == (char *)-1)
    {
        perror("shmat");
        return;
    }

    bzero((void *)shm, KEYCACHESZ);

    //qDebug("KB SHM init!");

    ui->setupUi(this);
    vga = QImage(800, 600, QImage::Format_RGB32);
    vga.fill(255);
    //qDebug("Loaded!");

    screen = QPixmap::fromImage(vga);

    scene = new QGraphicsScene(this);
    scene->addPixmap(screen);
    scene->setSceneRect(screen.rect());
    ui->graphicsView->setScene(scene);
}


void MainWindow::refresh()
{
    //qDebug("void MainWindow::refresh()!");
    /*
    QRgb value;
    value = qRgb(189, 149, 39); // 0xffbd9527
    vga.setPixel(1, 1, value);

    value = qRgb(122, 163, 39); // 0xff7aa327
    vga.setPixel(0, 1, value);
    vga.setPixel(1, 0, value);

    value = qRgb(237, 187, 51); // 0xffedba31
    vga.setPixel(2, 1, value);
    */
    screen = QPixmap::fromImage(vga);
    scene->addPixmap(screen);
    scene->setSceneRect(screen.rect());
    scene->update();

    //ui->currentWindow->update();
    //ui->graphicsView->update();

    //ui->graphicsView->setScene(scene);
    //ui->graphicsView->update();
    //scene->update(QRectF(0,0,vga.width(), this->vga.height()));
    //this->update();
    //ui->currentWindow->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}
