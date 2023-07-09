#include "roomresult.h"
#include "ui_roomresult.h"

RoomResult::RoomResult(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomResult)
{
    ui->setupUi(this);
}

RoomResult::RoomResult(QWidget *parent, std::string roomId, std::string level, std::string start) :
    QDialog(parent),
    ui(new Ui::RoomResult)
{
    ui->setupUi(this);
    ui->roomId->setText(QString::fromStdString(roomId);
    QString prefix = QString("color: rgb(255, 255, 255);border-radius: 10px;");
    if(level == "EASY")
        ui->level->setStyleSheet(QString(prefix + "background-color: #649696;"));
    else if(level == "MEDIUM")
        ui->level->setStyleSheet(QString(prefix + "background-color: rgb(244, 175, 15);"));
    else if(level == "HARD")
        ui->level->setStyleSheet(QString(prefix + "background-color: rgb(172, 3, 3);"));
    ui->level->setText(QString::fromStdString(level));
    ui->time->setText(QString::fromStdString(start));
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","get_test_summary", allocator);
    document.AddMember("room_id",tojsonValueType(roomId,allocator) , allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string jsonString = buffer.GetString();
    char* res = Connect::getInstance()->both(jsonString);
    std::string str(res);
    qDebug() << QString::fromStdString(str);
    rapidjson::Document doc;
    doc.Parse(res);
    if (doc.IsObject()) {
        if (doc.HasMember("students") && doc["students"].IsArray()) {
            const rapidjson::Value& dataArray = doc["students"].GetArray();
            ui->resultTable->setRowCount(dataArray.Size());
            for (rapidjson::SizeType i = 0; i < dataArray.Size(); ++i) {
                const rapidjson::Value& object = dataArray[i];
                if (object.IsObject()) {
                    QTableWidgetItem *newItem;
                    if (object.HasMember("username") && object["username"].IsString()) {
                        const std::string id = object["username"].GetString();
                        newItem = new QTableWidgetItem(QString::fromStdString(id));
                        newItem->setTextAlignment(4);
                        ui->resultTable->setItem(i, 0, newItem);
                    }

                    if (object.HasMember("end_time") && object["end_time"].IsInt()) {
                        const long int time= object["end_time"].GetInt64();
                        time_t timestamp = static_cast<time_t>(time);
                        std::string formattedTime;
                        formattedTime.resize(100); // Reserve space for the formatted time
                        std::strftime(&formattedTime[0], formattedTime.size(), "%Y-%m-%d %H:%M", std::localtime(&timestamp));
                        formattedTime.resize(std::strlen(formattedTime.c_str())); // Trim the string to fit the actual length
                        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(formattedTime));
                        newItem->setTextAlignment(4);
                        ui->resultTable->setItem(i, 1, newItem);
                    }

                    if (object.HasMember("points") && object["points"].IsInt()) {
                        const int totalPoints = object["points"].GetInt();
                        newItem = new QTableWidgetItem(QString::number(totalPoints));
                        newItem->setTextAlignment(4);
                        ui->resultTable->setItem(i, 2, newItem);
                    }

                }
            }
        }
    }
}

RoomResult::~RoomResult()
{
    delete ui;
}

