#include "exam.h"
#include "ui_exam.h"
#include "../Question/questionframe.h"
#include <cmath>
#include <QStackedWidget>
#include <QMessageBox>
#include <QDebug>
#include <ctime>

Exam::Exam(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    ui->setupUi(this);
}

Exam::Exam(QWidget *parent, std::vector<Question*> qList, int time,std::string level) :
    QWidget(parent),
    ui(new Ui::Exam)
{
    float size =qList.size();
    int amountPage = std::ceil(size/5);
    ui->setupUi(this);
    this->level = level;
    ui->level->setText(QString::fromStdString(level));
    QString prefix = QString("color: rgb(255, 255, 255);border-radius: 10px;");
    if(level == "EASY")
        ui->level->setStyleSheet(QString(prefix + "background-color: #649696;"));
    else if(level == "MEDIUM")
        ui->level->setStyleSheet(QString(prefix + "background-color: rgb(244, 175, 15);"));
    else if(level == "HARD")
        ui->level->setStyleSheet(QString(prefix + "background-color: rgb(172, 3, 3);"));
    std::time_t currentTime = std::time(nullptr);
    std::string formattedTime;
    formattedTime.resize(100); // Reserve space for the formatted time
    std::strftime(&formattedTime[0], formattedTime.size(), "%Y-%m-%d %H:%M", std::localtime(&currentTime));
    formattedTime.resize(std::strlen(formattedTime.c_str())); // Trim the string to fit the actual length
    this->startTime = formattedTime;

    for(int i = 1; i <= 40; ++i){
        auto btn = findChild<QPushButton*>("qBtn_"+QString::number(i));
        if(i>qList.size()){
            btn->hide();
            continue;
        }
        connect(btn, &QPushButton::clicked,this,[=](){
            ui->stackedWidget->setCurrentIndex(ceil((i-1)/5));
        });
        this->btnList.push_back(btn);
    }
    for(int i = 0; i < amountPage;i++){
        auto questionsBox = findChild<QVBoxLayout*>("questionsBox_"+QString::number(i+1));
        for(int j = 0; j < 5 && i*5+j <qList.size(); j++){
        QuestionFrame* qFrame = new QuestionFrame(this,qList[i*5+j],btnList[i*5+j]);
        questionsBox->addWidget(qFrame);
        }
    }
    ui->prevPageBtn->setText("");

    connect(ui->prevPageBtn, &QPushButton::clicked, this, [=](){
        int current = ui->stackedWidget->currentIndex();
        if(current >0){
            ui->stackedWidget->setCurrentIndex(current-1);
        }
    });
    connect(ui->nextPageBtn, &QPushButton::clicked, this, [=](){
        int current = ui->stackedWidget->currentIndex();
        if(current < amountPage-1){
            ui->stackedWidget->setCurrentIndex(current+1);
        }
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](){
        int current = ui->stackedWidget->currentIndex();
        if(current <= 0 ){
           ui->prevPageBtn->setText("");
        }else{
           ui->prevPageBtn->setText("<< Prev Page");
        }

        if(current >= amountPage-1 ){
           ui->nextPageBtn->setText("");
        }else{
           ui->nextPageBtn->setText("Next Page >>");
        }
    });

    connect(ui->submit, &QPushButton::clicked, this, [=](){
        int ret = QMessageBox::warning(this, tr("My Application"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                           QMessageBox::Yes | QMessageBox::Cancel);

        if(ret == QMessageBox::Yes){
            timer->stop();
            emit submit();
        }

    });
    connect(this->timer,SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->setSingleShot(true);
    QTime n(0,0,0);
    *duration = n.addSecs(0);
    *displayTime = n.addSecs(time);
    ui->time->setText(displayTime->toString("hh:mm:ss"));
    if(displayTime->hour() != 0 ||
        displayTime->minute() != 0 ||
        displayTime->second() != 0)
    {
        timer->start(1000);
    }
    else
    {
        timer->stop();
    }
}
void Exam::updateTime()
{

    QTime newTime = displayTime->addSecs(-1);
    displayTime->setHMS(newTime.hour(),newTime.minute(),newTime.second());
    ui->time->setText(displayTime->toString("hh:mm:ss"));
    newTime = duration->addSecs(1);
    duration->setHMS(newTime.hour(),newTime.minute(),newTime.second());
    if(displayTime->hour() != 0 ||
        displayTime->minute() != 0 ||
        displayTime->second() != 0)
    {
        timer->start(1000);
    }
    else
    {
        emit submit();
        timer->stop();
    }
}

std::string Exam::getStartTime(){
    return this->startTime;
}
std::string Exam::getLevel(){
    return this->level;
}
std::string Exam::getDuration(){
    return this->duration->toString("hh:mm:ss").toStdString();
}
int Exam::getDurationInt(){
    return this->duration->second();
}
Exam::~Exam()
{
    delete ui;
}
