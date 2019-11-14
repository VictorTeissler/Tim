
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update_log(const QString &data)
{
  //  this->ui->
    this->ui->textBrowser_2->append(data);
}


