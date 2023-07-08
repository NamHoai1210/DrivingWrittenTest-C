#include "questionframe.h"
#include "ui_questionframe.h"


QuestionFrame::QuestionFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuestionFrame)
{
    ui->setupUi(this);
}

QuestionFrame::~QuestionFrame()
{
    delete ui;
    for(int i=0; i<this->rList.size();i++){
        delete rList[i];
    }
}

QuestionFrame::QuestionFrame(QWidget *parent, Question *question, QPushButton *btn):
    QWidget(parent),
    ui(new Ui::QuestionFrame)
{
    ui->setupUi(this);
    this->question = question;
    ui->question->setText(QString::fromStdString(question->getQuestion()));
    for(int i=0;i<question->getAnswers().size();i++){
        QRadioButton *button = new QRadioButton("", this);
        this->rList.push_back(button);
        connect(button,&QRadioButton::clicked,btn,[=](){
            btn->setStyleSheet(QString("QPushButton{background-color: white;} QPushButton:hover{background-color: lightgray;}"));
            question->setTrue(i);
        });
        QLabel * label = new QLabel(QString::fromStdString(question->getAnswer(i)),this);
        label->setWordWrap(true);
        label->setMaximumWidth(600);
        label->setMinimumWidth(600);
        label->setMaximumHeight(300);
        ui->answersLayout->addRow(button,label);
    }
    connect(ui->clear,&QPushButton::clicked,this,[=](){
        for(int i=0; i<this->rList.size();i++){
            this->rList[i]->setAutoExclusive(false);
            this->rList[i]->setChecked(false);
            this->rList[i]->setAutoExclusive(true);
        }
        question->setTrue(-1);
        btn->setStyleSheet(QString(""));
    });

}

Question* QuestionFrame::getQuestion(){
    return this->question;
}

void QuestionFrame::setQuestion(Question *question){
    this->question = question;
}
