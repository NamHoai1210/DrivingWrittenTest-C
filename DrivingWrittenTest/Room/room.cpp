#include "room.h"
#include "ui_room.h"

Room::Room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setXOffset(2);
    effect->setYOffset(4);
    effect->setBlurRadius(8);
    effect->setColor(QColor(200, 200, 200, 180));

    setGraphicsEffect(effect);
}

Room::~Room()
{
    delete ui;
}
