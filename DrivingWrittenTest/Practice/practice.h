#ifndef PRACTICE_H
#define PRACTICE_H
#include "../Entity/Question/question.h"
#include <QMainWindow>
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "../Exam/exam.h"
#include "../Utils/utils.h"

QT_BEGIN_NAMESPACE
namespace  Ui {
class PracticeWindow;
}
QT_END_NAMESPACE

enum Level{
    HARD = 2,
    MEDIUM=1,
    EASY=0,
};

class PracticeWindow: public QMainWindow{
    Q_OBJECT
private:
    std::vector<Question*> qListEasy;
    std::vector<Question*> qListMedium;
    std::vector<Question*> qListHard;
    std::vector<Question*> qList;
    std::vector<History*> history;
    std::vector<int> result;
    Exam * e;
    std::vector<Question*> getRandomElements(std::vector<Question*>&, int);

public:
    PracticeWindow(QWidget *parent = nullptr);
    ~PracticeWindow();
private slots:
    void Submit();
    void on_continueNav_clicked();

    void on_easy_clicked();

    void on_medium_clicked();

    void on_hard_clicked();

private:
    Ui::PracticeWindow *ui;
    void generateQList(int qNum = 40, Level level = EASY);
    void initTable();
};


#endif // PRACTICE_H
