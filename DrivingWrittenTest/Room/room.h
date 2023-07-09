#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include "../Entity/Room/roomentity.h"
#include <QTime>
#include <cstring>
#include <iostream>
#include <QDebug>
#include <ctime>
#include "../RoomResult/roomresult.h"
namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = nullptr, RoomEntity* room = nullptr);
    void updateStatus();
    ~Room();

private slots:
    void on_view_clicked();

    void on_join_clicked();

private:
    Ui::Room *ui;
    RoomEntity* room;
    RoomResult* result;
    std::string start;
signals:
    void join();
};

#endif // ROOM_H
