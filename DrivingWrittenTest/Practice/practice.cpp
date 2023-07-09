#include "practice.h"
#include "./ui_practice.h"
#include <string>
#include <QDebug>


using namespace rapidjson;
PracticeWindow::PracticeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PracticeWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);
    ui->ChooseStack->setCurrentIndex(0);
    Question::readQuestionFile(this->qListHard,":/db/hard.json");
    Question::readQuestionFile(this->qListMedium,":/db/medium.json");
    Question::readQuestionFile(this->qListEasy,":/db/easy.json");
    connect(ui->homeNav,&QPushButton::clicked,this,[=](){
        parent->show();
        this->close();
    });
    this->history = readHistory();
    this->initTable();
}
void PracticeWindow::Submit(){
    std::string duration = e->getDuration();
    std::string startTime = e->getStartTime();
    std::string level = e->getLevel();
    QString prefix = QString("color: rgb(255, 255, 255);border-radius: 10px;");
    if(level == "EASY")
        ui->level->setStyleSheet(QString(prefix + "background-color: #649696;"));
    else if(level == "MEDIUM")
        ui->level->setStyleSheet(QString(prefix + "background-color: rgb(244, 175, 15);"));
    else if(level == "HARD")
        ui->level->setStyleSheet(QString(prefix + "background-color: rgb(172, 3, 3);"));
    ui->duration->setText(QString::fromStdString(duration));
    ui->level->setText(QString::fromStdString(level));
    ui->time->setText(QString::fromStdString(startTime));
    ui->mainLayout->removeWidget(e);
    e->~Exam();
    ui->stackedWidget->setCurrentIndex(1);
    ui->ChooseStack->setCurrentIndex(1);
    float numTrue = 0;
    for(int i=0;i<this->qList.size();i++){
        if(i>=result.size()) break;
        if(result[i]== qList[i]->getTrue()) numTrue++;
    }
    ui->score->setText(QString::number(numTrue*2.5));
    History * h = new History(startTime,level,duration,numTrue*2.5);
    this->history.insert(this->history.begin(),h);
    writeHistory(this->history);
    initTable();
    for (Question* ptr : this->qList)
    {
        delete ptr;
    }
    this->qList.clear();
}
PracticeWindow::~PracticeWindow()
{
    delete ui;
    for (History* ptr : this->history){
        delete ptr;
    }
    this->history.clear();
    for (Question* ptr : this->qList)
    {
        delete ptr;
    }
    this->qList.clear();
    for (Question* ptr : this->qListHard)
    {
        delete ptr;
    }
    this->qListHard.clear();
    for (Question* ptr : this->qListMedium)
    {
        delete ptr;
    }
    this->qListMedium.clear();
    for (Question* ptr : this->qListEasy)
    {
        delete ptr;
    }
    this->qListEasy.clear();
}

void PracticeWindow::on_continueNav_clicked()
{
    ui->ChooseStack->setCurrentIndex(0);
}

void PracticeWindow::generateQList(int qNum, Level level){
    int qNumHard=0, qNumMedium=0, qNumEasy=0;
    if(level==EASY){
        qNumHard = 0;
        qNumMedium = qNum*5/10;
        qNumEasy = qNum*5/10;
    } else if(level==MEDIUM){
        qNumHard = qNum*2/10;
        qNumMedium = qNum*5/10;
        qNumEasy = qNum*3/10;
    } else if(level==HARD){
        qNumHard = qNum*4/10;
        qNumMedium = qNum*5/10;
        qNumEasy = qNum*1/10;
    }
    this->qList = std::vector<Question*>();
    this->result = std::vector<int>();
    std::vector<Question*> tmp = this->getRandomElements(this->qListEasy, qNumEasy);
    for(int i=0;i<tmp.size();i++){
        this->result.push_back(tmp[i]->getTrue());
        this->qList.push_back(new Question("Question "+std::to_string(qList.size()+1)+". "+tmp[i]->getQuestion(),tmp[i]->getAnswers()));
    }
    tmp = this->getRandomElements(this->qListMedium, qNumMedium);
    for(int i=0;i<tmp.size();i++){
        this->result.push_back(tmp[i]->getTrue());
        this->qList.push_back(new Question("Question "+std::to_string(qList.size()+1)+". "+tmp[i]->getQuestion(),tmp[i]->getAnswers()));
    }
    tmp = this->getRandomElements(this->qListHard, qNumHard);
    for(int i=0;i<tmp.size();i++){
        this->result.push_back(tmp[i]->getTrue());
        this->qList.push_back(new Question("Question "+std::to_string(qList.size()+1)+". "+tmp[i]->getQuestion(),tmp[i]->getAnswers()));
    }
    qDebug() << this->result;
}

std::vector<Question*> PracticeWindow::getRandomElements(std::vector<Question*>& array, int n)
{
    // Create a copy of the original array
    std::vector<Question*> shuffledArray = array;

    // Create a random number generator
    std::random_device rd;
    std::mt19937 generator(rd());

    // Perform Fisher-Yates shuffle
    for (int i = shuffledArray.size() - 1; i > 0; --i)
    {
        std::uniform_int_distribution<int> distribution(0, i);
        int j = distribution(generator);
        std::swap(shuffledArray[i], shuffledArray[j]);
    }

    // Return the first n elements as the random sample
    return std::vector<Question*>(shuffledArray.begin(), shuffledArray.begin() + n);
}

void PracticeWindow::on_easy_clicked()
{
    generateQList(40,EASY);
    this->e = new Exam(this,this->qList,60,"EASY");
    ui->stackedWidget->setCurrentIndex(0);
    connect(e,SIGNAL(submit()),this,SLOT(Submit()));
    ui->mainLayout->addWidget(e);
}


void PracticeWindow::on_medium_clicked()
{
    generateQList(40,MEDIUM);
    this->e = new Exam(this,this->qList,60,"MEDIUM");
    ui->stackedWidget->setCurrentIndex(0);
    connect(e,SIGNAL(submit()),this,SLOT(Submit()));
    ui->mainLayout->addWidget(e);
}


void PracticeWindow::on_hard_clicked()
{
    generateQList(40,HARD);
    this->e = new Exam(this,this->qList,60,"HARD");
    ui->stackedWidget->setCurrentIndex(0);
    connect(e,SIGNAL(submit()),this,SLOT(Submit()));
    ui->mainLayout->addWidget(e);
}

void PracticeWindow::initTable(){
    ui->historyTable->setRowCount(this->history.size());
    for(int i = 0; i< history.size();i++){
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(history[i]->getTime()));
        newItem->setTextAlignment(4);
        ui->historyTable->setItem(i, 0, newItem);
        newItem = new QTableWidgetItem(QString::fromStdString(history[i]->getLevel()));
        newItem->setTextAlignment(4);
        ui->historyTable->setItem(i, 1, newItem);
        newItem = new QTableWidgetItem(QString::fromStdString(history[i]->getDuration()));
        newItem->setTextAlignment(4);
        ui->historyTable->setItem(i, 2, newItem);
        newItem = new QTableWidgetItem(QString::number(history[i]->getScore()));
        newItem->setTextAlignment(4);
        ui->historyTable->setItem(i, 3, newItem);
    }
}

