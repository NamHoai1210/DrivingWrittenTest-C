#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>
#include <QDateTimeEdit>
#include "../Entity/Room/roomentity.h"
#include "../rapidjson/document.h"
#include "../rapidjson/istreamwrapper.h"
#include "../rapidjson/writer.h"
#include "../Utils/utils.h"
#include "../connect/connect.h"
#include "../session/session.h"
#include <cstring>

namespace Ui {
class CreateRoom;
}

class CreateRoom : public QDialog
{
    Q_OBJECT

public:
    explicit CreateRoom(QWidget *parent = nullptr);
    ~CreateRoom();

private:
    Ui::CreateRoom *ui;
    RoomEntity* room;
signals:
    void closing();
    void success();
private slots:
    void on_cancel_clicked();
    void on_level_currentIndexChanged(int index);
    void on_create_clicked();
};

#endif // CREATEROOM_H
