#include "onlinewindow.h"
#include "ui_onlinewindow.h"


OnlineWindow::OnlineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OnlineWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
//    QMovie *movie = new QMovie(":/resources/gif/waiting.gif");
//    ui->waitingGif->setMovie(movie);
//    movie->start();
    for(int i=0;i<5;i++){
        for(int j=0;j< 3;j++){
            Room *r = new Room();
            ui->listRoomLayout->addWidget(r,i,j,1,1);
        }
    }
}

OnlineWindow::~OnlineWindow()
{
    delete ui;
}
