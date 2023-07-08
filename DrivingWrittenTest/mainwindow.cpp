#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_practice_clicked()
{
    this->practiceWindow = new PracticeWindow(this);
    hide();
    practiceWindow->show();
}


void MainWindow::on_online_clicked()
{
    this->onlineWindow = new OnlineWindow(this);
    hide();
    onlineWindow->show();
}

