#ifndef EXAM_H
#define EXAM_H

#include <QWidget>
#include <vector>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include "../Entity/Question/question.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Exam;
}
QT_END_NAMESPACE
class Exam : public QWidget
{
    Q_OBJECT
private:
    Ui::Exam *ui;
private:
    std::vector<QPushButton*> btnList;
    std::string startTime;
    std::string level;
public:
    explicit Exam(QWidget *parent = nullptr);
    explicit Exam(QWidget *parent = nullptr, std::vector<Question*> = std::vector<Question*>(), int time = 0 ,std::string level = "EASY");
    ~Exam();
public:
    QTime *displayTime = new QTime;
    QTime *duration = new QTime;
    QTimer *timer = new QTimer;
    std::string getStartTime();
    std::string getLevel();
    std::string getDuration();
protected slots:
    void updateTime();
signals:
    void submit();
};

#endif // EXAM_H
