#include "roomresult.h"
#include "ui_roomresult.h"

RoomResult::RoomResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomResult)
{
    ui->setupUi(this);
}

RoomResult::~RoomResult()
{
    delete ui;
}
