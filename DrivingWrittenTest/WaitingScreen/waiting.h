#ifndef WAITING_H
#define WAITING_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include "../Entity/Room/roomentity.h"
#include "../session/session.h"
#include <QMovie>
namespace Ui {
class Waiting;
}

class Waiting : public QWidget
{
    Q_OBJECT

public:
    explicit Waiting(QWidget *parent = nullptr);
    void startCounter();
    void hideOutBtn();
    void showOutBtn();
    ~Waiting();

private:
    Ui::Waiting *ui;
    QTimer *waiting;
    QTime *displayTime = new QTime;
protected slots:
    void updateTime();
signals:
    void start();
    void out();
private slots:
    void on_outNav_clicked();
};

#endif // WAITING_H
