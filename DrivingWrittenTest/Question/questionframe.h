#ifndef QUESTIONFRAME_H
#define QUESTIONFRAME_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <vector>
#include "../Entity/Question/question.h"

namespace Ui {
class QuestionFrame;
}

class QuestionFrame : public QWidget
{
    Q_OBJECT
private:
    Question *question;
public:
    explicit QuestionFrame(QWidget *parent = nullptr, Question * question=nullptr, QPushButton * btn=nullptr);
    void setQuestion(Question *q);
    Question* getQuestion();

public:
    explicit QuestionFrame(QWidget *parent = nullptr);
    ~QuestionFrame();

private:
    Ui::QuestionFrame *ui;
    std::vector<QRadioButton*> rList;
};

#endif // QUESTIONFRAME_H
