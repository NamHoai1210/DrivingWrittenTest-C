#include "createroom.h"
#include "ui_createroom.h"

CreateRoom::CreateRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateRoom)
{
    ui->setupUi(this);
    room = new RoomEntity();
    this->room->setDuration(30);
    this->room->setTotalPoints(20);
    this->room->setRankPointLimit(0);
    ui->level->setCurrentIndex(0);
    ui->duration->setText("30 seconds");
    ui->no->setText("20 questions");
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDateTime later = currentDateTime.addSecs(7200);
    ui->limit->setText(QString::number(room->getRankPointLimit())+" point(s)");
    ui->startTime->setMinimumDateTime(currentDateTime);
    ui->startTime->setMaximumDateTime(later);
}

CreateRoom::~CreateRoom()
{
    delete ui;
}

void CreateRoom::on_cancel_clicked()
{
    emit closing();
}


void CreateRoom::on_level_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        this->room->setDuration(30);
        this->room->setTotalPoints(20);
        this->room->setRankPointLimit(0);
        break;
    case 1:
        this->room->setDuration(60);
        this->room->setTotalPoints(30);
        this->room->setRankPointLimit(100);
        break;
    case 2:
        this->room->setDuration(600);
        this->room->setTotalPoints(40);
        this->room->setRankPointLimit(200);
        break;
    default:
        break;
    }
    ui->duration->setText(QString::number(room->getDuration())+" second(s)");
    ui->no->setText(QString::number(room->getTotalPoints())+" question(s)");
    ui->limit->setText(QString::number(room->getRankPointLimit())+" point(s)");
}


void CreateRoom::on_create_clicked()
{
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","create_room", allocator);

    QString name = ui->name->text();
    document.AddMember("name",tojsonValueType(name.toStdString(),allocator) , allocator);
    QDateTime selectedDateTime = ui->startTime->dateTime();
    time_t selectedTime = selectedDateTime.toTime_t();
    rapidjson::Value timeValue(static_cast<int64_t>(selectedTime));
    document.AddMember("time_start", timeValue, allocator);
    rapidjson::Value durationValue(room->getDuration());
    document.AddMember("duration",durationValue, allocator);
    rapidjson::Value totalPointsValue(room->getTotalPoints());
    document.AddMember("total_points",totalPointsValue, allocator);
    rapidjson::Value limitValue(room->getRankPointLimit());
    document.AddMember("rank_point_limit",limitValue, allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string jsonString = buffer.GetString();
    char* res = Connect::getInstance()->both(jsonString);
    rapidjson::Document doc;
    doc.Parse(res);
    if (doc.HasMember("status") && doc["status"].IsString()) {
        const std::string status = doc["status"].GetString();
        if(status == "success"){
            ui->error->setText("");
            if (doc.HasMember("room_id") && doc["room_id"].IsString()){
               const std::string roomId = doc["room_id"].GetString();
                this->room->setId(roomId);
               this->room->setName(name.toStdString());
               this->room->setTimeStart(selectedTime);
               this->room->setLevel(ui->level->currentText().toStdString());
               Session::getInstance()->saveRoom(this->room);
                emit success();
            }
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

