#include "mainwindow.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->signout->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSuccess(std::string username){
    qDebug() <<"catch";
    this->authWindow->close();
    ui->online->setText("Return ");
    ui->signout->show();
    Session::getInstance()->saveSession(username);
    this->onlineWindow = new OnlineWindow(this);
    onlineWindow->show();
}
void MainWindow::on_practice_clicked()
{
    this->practiceWindow = new PracticeWindow(this);
    hide();
    practiceWindow->show();
}


void MainWindow::on_online_clicked()
{
    if(Session::getInstance()->getUsername()==""){
        Connect::getInstance()->connectToServer();
        this->authWindow = new AuthWindow(this);
        connect(authWindow,SIGNAL(success(std::string)),this,SLOT(onSuccess(std::string)));
        hide();
        authWindow->show();
    }else{
    this->onlineWindow = new OnlineWindow(this);
    hide();
    onlineWindow->show();
    }
}


void MainWindow::on_signout_clicked()
{
    Session::getInstance()->saveSession("");
    ui->online->setText("Login ");
    ui->signout->hide();
    Connect::getInstance()->disconnectToServer();
}

