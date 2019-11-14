#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <qgraphicsscene.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <strings.h>

#define KEYCACHESZ 32
#define SHMSZ (800*600+1)*4 //1 for the control byte at the end

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    QImage vga;
    QGraphicsScene* scene;
    QPixmap screen;
    Ui::MainWindow* ui;
    char* shm;

  public slots:
    void refresh();
    void keyPressEvent(QKeyEvent*);
    void keyReleaseEvent(QKeyEvent*);
};

#endif // MAINWINDOW_H
