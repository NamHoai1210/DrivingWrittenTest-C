#include "room.h"
#include "ui_room.h"

Room::Room(QWidget *parent, RoomEntity* roome):
    QWidget(parent),
    ui(new Ui::Room)
{
    this->result = nullptr;
    ui->setupUi(this);
    ui->error->setText("");
    this->room = roome;
    QTime n(0,0,0);
    QTime duration = n.addSecs(roome->getDuration());
    ui->durationRoom->setText(duration.toString("hh:mm:ss"));
    std::time_t currentTime = roome->getTimeStart();
    qDebug()<< QString::number(currentTime);
    std::string formattedTime;
    formattedTime.resize(100); // Reserve space for the formatted time
    std::strftime(&formattedTime[0], formattedTime.size(), "%Y-%m-%d %H:%M", std::localtime(&currentTime));
    formattedTime.resize(std::strlen(formattedTime.c_str())); // Trim the string to fit the actual length
    start = formattedTime;
    ui->timeRoom->setText(QString::fromStdString(formattedTime));
    ui->roomName->setText(QString::fromStdString(roome->getName()));
    ui->level->setText(QString::number(roome->getRankPointLimit()));
    time_t current = std::time(nullptr);
    if(current<currentTime){
        ui->statusStack->setCurrentIndex(0);
    }else if(current >= currentTime + roome->getDuration()){
        ui->statusStack->setCurrentIndex(2);
    }else {
        ui->statusStack->setCurrentIndex(1);
    }
    if(this->room->getTotalPoints()==20){
        this->room->setLevel("EASY");
    }else if(this->room->getTotalPoints()==30){
        this->room->setLevel("MEDIUM");
    }else if(this->room->getTotalPoints()==40){
        this->room->setLevel("HARD");
    }

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setXOffset(2);
    effect->setYOffset(4);
    effect->setBlurRadius(8);
    effect->setColor(QColor(200, 200, 200, 180));
    setGraphicsEffect(effect);
}

void Room::updateStatus(){
    time_t current = std::time(nullptr);
    if(current<room->getTimeStart()){
        ui->statusStack->setCurrentIndex(0);
    }else if(current >= room->getTimeStart() + room->getDuration()){
        ui->statusStack->setCurrentIndex(2);
    }else {
        ui->statusStack->setCurrentIndex(1);
    }
}

Room::~Room()
{
    delete ui;
}


void Room::on_view_clicked()
{
    if(this->result == nullptr){
        this->result = new RoomResult(this,this->room->getId(), this->room->getLevel(), start);
    }
    this->result->show();
}


void Room::on_join_clicked()
{
    const int rankPoint = Session::getInstance()->getRankPoint();
    if(rankPoint < this->room->getRankPointLimit()){
        ui->error->setText("Not enough rank point");
        return;
    }
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","join_room", allocator);
    std::string username = Session::getInstance()->getUsername();
    document.AddMember("username",tojsonValueType(username,allocator) , allocator);
    document.AddMember("room_id",tojsonValueType(this->room->getId(),allocator) , allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string jsonString = buffer.GetString();
    char* res = Connect::getInstance()->both(jsonString);
    std::string str(res);
    qDebug() << QString::fromStdString(str);
    rapidjson::Document doc;
    doc.Parse(res);
    if (doc.HasMember("status") && doc["status"].IsString()) {
        const std::string status = doc["status"].GetString();
        if(status == "success"){
            ui->error->setText("");
            Session::getInstance()->saveRoom(this->room);
            emit join();
            return;

        }else{
            if (doc.HasMember("message") && doc["message"].IsString()) {
                const std::string message = doc["message"].GetString();
                ui->error->setText(QString::fromStdString(message));
                return;
            }
        }
    }
    ui->error->setText("Unknow error");
    return;
}

