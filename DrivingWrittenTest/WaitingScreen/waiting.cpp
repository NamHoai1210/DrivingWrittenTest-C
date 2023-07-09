#include "waiting.h"
#include "ui_waiting.h"
#include <QDebug>
Waiting::Waiting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Waiting)
{
    this->waiting = new QTimer();
    ui->setupUi(this);
    connect(this->waiting,SIGNAL(timeout()), this,SLOT(updateTime()));
}

Waiting::~Waiting()
{
    delete ui;
}

void Waiting::startCounter(){
    QMovie *movie = new QMovie(":/resources/gif/waiting.gif");
    ui->waitingGif->setMovie(movie);
    movie->start();
    ui->roomId->setText(QString::fromStdString(Session::getInstance()->getCurrentRoom()->getName()));
    QTime n(0,0,0);
    RoomEntity * curroom= Session::getInstance()->getCurrentRoom();
    int diffInSeconds = static_cast<int>(difftime(curroom->getTimeStart(),std::time(nullptr)));
    if(diffInSeconds>0){
        *displayTime = n.addSecs(diffInSeconds);
        ui->time->setText(displayTime->toString("hh:mm:ss"));
        if(displayTime->hour() != 0 ||
            displayTime->minute() != 0 ||
            displayTime->second() != 0)
        {
            waiting->start(1000);
        }else{
            emit start();
        }
    }else emit start();
}

void Waiting::updateTime()
{
    QTime newTime = displayTime->addSecs(-1);
    displayTime->setHMS(newTime.hour(),newTime.minute(),newTime.second());
    ui->time->setText(displayTime->toString("hh:mm:ss"));
    if(displayTime->hour() != 0 ||
        displayTime->minute() != 0 ||
        displayTime->second() != 0)
    {
        waiting->start(1000);
    }
    else
    {
        waiting->stop();
        emit start();
    }
}

void Waiting::on_outNav_clicked()
{
    waiting->stop();
    emit out();
}

void Waiting::hideOutBtn(){
    ui->outNav->hide();
}

void Waiting::showOutBtn(){
    ui->outNav->show();
}
