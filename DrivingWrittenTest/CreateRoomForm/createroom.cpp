#include "createroom.h"
#include "ui_createroom.h"

CreateRoom::CreateRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateRoom)
{
    ui->setupUi(this);
}

CreateRoom::~CreateRoom()
{
    delete ui;
}
