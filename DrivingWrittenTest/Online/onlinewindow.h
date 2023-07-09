#ifndef ONLINEWINDOW_H
#define ONLINEWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include "../Room/room.h"
#include "../session/session.h"
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/writer.h"
#include <vector>
#include "../Utils/utils.h"
#include "../connect/connect.h"
#include <QTimer>
#include <QTime>
#include <cmath>
#include "../CreateRoomForm/createroom.h"
#include "../WaitingScreen/waiting.h"
#include "../Entity/Question/question.h"
#include "../Question/questionframe.h"
#include "../Exam/exam.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class OnlineWindow;
}
QT_END_NAMESPACE
class OnlineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OnlineWindow(QMainWindow *parent = nullptr);
    ~OnlineWindow();

private slots:
    void on_createRoom_3_clicked();

    void on_historyNav_clicked();

    void on_createRoom_2_clicked();

    void on_createRoom_clicked();

    void on_toHomeNav_clicked();

private:
    Ui::OnlineWindow *ui;
    QMainWindow* parent;
    std::vector<RoomEntity*> roomList;
    QTimer *timer;
    std::vector<RoomEntity*> getListRoom();
    CreateRoom* createRoom;
    Waiting* w;
    std::vector<int> getListQuestion();
    std::vector<Question*> qList;
    std::vector<int> result;
    Exam * e;
    std::vector<History*> history;
    bool SubmitAnswer(int, int);
    void getHistory();
    void initTable();

protected slots:
    void updateRoomList();
    void onCloseDialog();
    void onSuccess();
    void onStartTest();
    void onOutRoom();
    void Submit();
    void onJoinRoom();
};

#endif // ONLINEWINDOW_H
