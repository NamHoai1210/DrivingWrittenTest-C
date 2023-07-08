#ifndef ROOMRESULT_H
#define ROOMRESULT_H

#include <QDialog>

namespace Ui {
class RoomResult;
}

class RoomResult : public QDialog
{
    Q_OBJECT

public:
    explicit RoomResult(QWidget *parent = nullptr);
    ~RoomResult();

private:
    Ui::RoomResult *ui;
};

#endif // ROOMRESULT_H
