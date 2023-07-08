#ifndef CREATEROOM_H
#define CREATEROOM_H

#include <QDialog>

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
};

#endif // CREATEROOM_H
