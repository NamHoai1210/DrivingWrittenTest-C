#include "onlinewindow.h"
#include "ui_onlinewindow.h"


OnlineWindow::OnlineWindow(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::OnlineWindow)
{
    timer = new QTimer();
    ui->setupUi(this);
    w = new Waiting();
    connect(w, SIGNAL(start()), this, SLOT(onStartTest()));
    connect(w, SIGNAL(out()), this, SLOT(onOutRoom()));
    ui->waitingLayout->addWidget(w);
    w->show();
    ui->stackedWidget->setCurrentIndex(0);
    ui->historyNav->setText(QString::fromStdString(Session::getInstance()->getUsername()));
    this->parent = parent;
    connect(this->timer,SIGNAL(timeout()), this, SLOT(updateRoomList()));
    timer->start(15000);
    this->roomList= getListRoom();
    float size =roomList.size();
    for(int i=0;i<std::ceil(size/3);i++){
        for(int j=0;j< 3 && i*3 +j < roomList.size();j++){
            Room *r = new Room(this,roomList[i*3+j]);
            connect(r,SIGNAL(join()),this,SLOT(onJoinRoom()));
            ui->listRoomLayout->addWidget(r,i,j,1,1);
        }
    }
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](){
        int current = ui->stackedWidget->currentIndex();
        if(current>0){
            this->timer->stop();
            if(current==2){
                w->startCounter();
            }
        }else {
            this->updateRoomList();
        }
    });
}

OnlineWindow::~OnlineWindow()
{
    delete ui;
    timer->stop();
    delete timer;
    for (RoomEntity* ptr : this->roomList){
        delete ptr;
    }
    for (History* ptr : this->history){
        delete ptr;
    }
    this->roomList.clear();
}

void OnlineWindow::on_createRoom_3_clicked()
{
    hide();
    this->parent->show();
}

std::vector<RoomEntity*> OnlineWindow::getListRoom(){
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","get_all_rooms", allocator);
    std::string username = Session::getInstance()->getUsername();
    document.AddMember("username",tojsonValueType(username,allocator) , allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    std::string jsonString = buffer.GetString();
    char* res = Connect::getInstance()->both(jsonString);
    std::string str(res);
    qDebug() << QString::fromStdString(str);
    rapidjson::Document doc;
    doc.Parse(res);
    std::vector<RoomEntity*> roomList;
    if (doc.IsObject()) {
        if (doc.HasMember("rooms") && doc["rooms"].IsArray()) {
            const rapidjson::Value& dataArray = doc["rooms"].GetArray();

            for (rapidjson::SizeType i = 0; i < dataArray.Size(); ++i) {
                const rapidjson::Value& object = dataArray[i];
                RoomEntity* room = new RoomEntity();
                if (object.IsObject()) {
                    if (object.HasMember("id") && object["id"].IsString()) {
                        const std::string id = object["id"].GetString();
                        room->setId(id);
                    }

                    if (object.HasMember("name") && object["name"].IsString()) {
                        const std::string name = object["name"].GetString();
                        room->setName(name);
                    }

                    if (object.HasMember("time_start") && object["time_start"].IsInt64()) {
                        const long int time= object["time_start"].GetInt64();
                         time_t timestamp = static_cast<time_t>(time);
                        room->setTimeStart(timestamp);
                    }

                    if (object.HasMember("duration") && object["duration"].IsInt()) {
                        const int duration = object["duration"].GetInt();
                        room->setDuration(duration);
                    }

                    if (object.HasMember("total_points") && object["total_points"].IsInt()) {
                        const int totalPoints = object["total_points"].GetInt();
                        room->setTotalPoints(totalPoints);
                    }

                    if (object.HasMember("rank_point_limit") && object["rank_point_limit"].IsInt()) {
                        const int rankPointLimit = object["rank_point_limit"].GetInt();
                        room->setRankPointLimit(rankPointLimit);
                    }
                }
                roomList.push_back(room);
            }
        }
    }
    return roomList;
}

void OnlineWindow::updateRoomList(){
    qDebug() << "catch";
    for (int row = 0; row < ui->listRoomLayout->rowCount(); ++row) {
        for (int column = 0; column < ui->listRoomLayout->columnCount(); ++column) {
            QLayoutItem* item = ui->listRoomLayout->itemAtPosition(row, column);
            if (item && item->widget()) {
                Room* room = dynamic_cast<Room*>(item->widget());
                if(room) room->updateStatus();
            }
        }
    }

    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","get_all_rooms", allocator);
    std::string username = Session::getInstance()->getUsername();
    document.AddMember("username",tojsonValueType(username,allocator) , allocator);
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
        if (doc.HasMember("rooms") && doc["rooms"].IsArray()) {
            const rapidjson::Value& dataArray = doc["rooms"].GetArray();

            for (rapidjson::SizeType i = this->roomList.size() ; i < dataArray.Size(); ++i) {
                const rapidjson::Value& object = dataArray[i];
                RoomEntity* room = new RoomEntity();
                if (object.IsObject()) {
                    if (object.HasMember("id") && object["id"].IsString()) {
                        const std::string id = object["id"].GetString();
                        room->setId(id);
                    }

                    if (object.HasMember("name") && object["name"].IsString()) {
                        const std::string name = object["name"].GetString();
                        room->setName(name);
                    }

                    if (object.HasMember("time_start") && object["time_start"].IsInt64()) {
                        const long int time= object["time_start"].GetInt64();
                        time_t timestamp = static_cast<time_t>(time);
                        room->setTimeStart(timestamp);
                    }

                    if (object.HasMember("duration") && object["duration"].IsInt()) {
                        const int duration = object["duration"].GetInt();
                        room->setDuration(duration);
                    }

                    if (object.HasMember("total_points") && object["total_points"].IsInt()) {
                        const int totalPoints = object["total_points"].GetInt();
                        room->setTotalPoints(totalPoints);
                    }

                    if (object.HasMember("rank_point_limit") && object["rank_point_limit"].IsInt()) {
                        const int rankPointLimit = object["rank_point_limit"].GetInt();
                        room->setRankPointLimit(rankPointLimit);
                    }
                }
                this->roomList.push_back(room);
                Room *r = new Room(this,room);
                connect(r,SIGNAL(join()),this,SLOT(onJoinRoom()));
                float tmp = i;
                float tmp2 = ceil((tmp+1)/3)-1;
                ui->listRoomLayout->addWidget(r,tmp2,tmp-tmp2*3,1,1);
            }
        }
    }
    timer->start(15000);

}

void OnlineWindow::on_historyNav_clicked()
{
    timer->stop();
    ui->stackedWidget->setCurrentIndex(1);
    if(this->history.size() == 0){
        getHistory();
        initTable();
    }
    ui->OnlineStack->setCurrentIndex(0);
}


void OnlineWindow::on_createRoom_2_clicked()
{
    timer->stop();
    this->updateRoomList();
}


void OnlineWindow::on_createRoom_clicked()
{
    this->createRoom = new CreateRoom();
    connect(createRoom,SIGNAL(closing()),this,SLOT(onCloseDialog()));
    connect(createRoom,SIGNAL(success()),this,SLOT(onSuccess()));
    this->createRoom->setModal(true);
    this->createRoom->exec();
}

void OnlineWindow::onCloseDialog(){
    qDebug() <<"closing";
   this->createRoom->hide();
}

void OnlineWindow::onSuccess(){
    qDebug() <<"success";
    this->createRoom->hide();
    this->w->hideOutBtn();
    ui->stackedWidget->setCurrentIndex(2);
}

void OnlineWindow::onOutRoom(){
    ui->stackedWidget->setCurrentIndex(0);
}

void OnlineWindow::onStartTest(){
    qDebug() << "start";
    this->result = getListQuestion();
    qDebug() << this->result;
    this->e = new Exam(this,this->qList,Session::getInstance()->getCurrentRoom()->getDuration(),Session::getInstance()->getCurrentRoom()->getLevel());
    ui->stackedWidget->setCurrentIndex(3);
    connect(e,SIGNAL(submit()),this,SLOT(Submit()));
    ui->mainLayout->addWidget(e);
}
std::vector<int> OnlineWindow::getListQuestion(){
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","start_test", allocator);
    std::string roomId = Session::getInstance()->getCurrentRoom()->getId();
    std::string username = Session::getInstance()->getUsername();
    document.AddMember("username",tojsonValueType(username,allocator) , allocator);
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
    std::vector<int> result;
    if (doc.IsObject()) {
        if (doc.HasMember("questions") && doc["questions"].IsArray()) {
            const rapidjson::Value& dataArray = doc["questions"].GetArray();

            for (rapidjson::SizeType i = 0 ; i < dataArray.Size(); ++i) {
                const rapidjson::Value& object = dataArray[i];
                Question* q = new Question();
                if (object.IsObject()) {
                    if (object.HasMember("text") && object["text"].IsString()) {
                        const std::string text = object["text"].GetString();
                        q->setQuestion("Question "+std::to_string(i+1)+". "+text);
                    }

                    if (object.HasMember("correct_choice") && object["correct_choice"].IsInt()) {
                        const int correctChoice = object["correct_choice"].GetInt();
                        q->setTrue(-1);
                        result.push_back(correctChoice);
                    }
                    if (object.HasMember("choices") && object["choices"].IsArray()) {
                        const rapidjson::Value& answers = object["choices"];

                        for (rapidjson::SizeType i = 0 ; i < answers.Size(); ++i) {
                            const rapidjson::Value& answer = answers[i];
                            if(answer.IsString()){
                                const std::string a = answer.GetString();
                                q->addAnswer(a);
                            }
                        }
                    }

                }
                this->qList.push_back(q);
            }
        }
    }
    return result;
}

void OnlineWindow::Submit(){
    int durationInt = e->getDurationInt();
    std::string duration = e->getDuration();
    std::string startTime = e->getStartTime();
    std::string level = e->getLevel();
    QString prefix = QString("color: rgb(255, 255, 255);border-radius: 10px;");
    if(level == "EASY")
        ui->testLevel->setStyleSheet(QString(prefix + "background-color: #649696;"));
    else if(level == "MEDIUM")
        ui->testLevel->setStyleSheet(QString(prefix + "background-color: rgb(244, 175, 15);"));
    else if(level == "HARD")
        ui->testLevel->setStyleSheet(QString(prefix + "background-color: rgb(172, 3, 3);"));
    ui->testDuration->setText(QString::fromStdString(duration));
    ui->testLevel->setText(QString::fromStdString(level));
    ui->testTime->setText(QString::fromStdString(startTime));
    ui->mainLayout->removeWidget(e);
    e->~Exam();
    ui->stackedWidget->setCurrentIndex(1);
    ui->OnlineStack->setCurrentIndex(1);
    float numTrue = 0;
    for(int i=0;i<this->qList.size();i++){
        if(i>=result.size()) break;
        if(result[i]== qList[i]->getTrue()) numTrue++;
    }
    ui->testScore->setText(QString::number(numTrue));
    bool check = SubmitAnswer(numTrue, durationInt);
    if(check){
        getHistory();
        initTable();
    }

    for (Question* ptr : this->qList)
    {
        delete ptr;
    }
    this->qList.clear();
}

bool OnlineWindow::SubmitAnswer(int point, int duration){
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","submit_answer", allocator);
    std::string name = Session::getInstance()->getUsername();
    document.AddMember("username",tojsonValueType(name,allocator) , allocator);
    std::string roomId = Session::getInstance()->getCurrentRoom()->getId();
    document.AddMember("room_id",tojsonValueType(roomId,allocator) , allocator);
    time_t selectedTime = Session::getInstance()->getCurrentRoom()->getTimeStart();
    rapidjson::Value timeValue(static_cast<int64_t>(selectedTime));
    document.AddMember("start_time", timeValue, allocator);
    time_t endTime = selectedTime + static_cast<time_t>(duration);
    rapidjson::Value endValue(static_cast<int64_t>(endTime));
    document.AddMember("end_time", endValue, allocator);
    document.AddMember("points",point, allocator);
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
            ui->submitError->setText("");
            return true;
        }else{
            if (doc.HasMember("message") && doc["message"].IsString()) {
                const std::string message = doc["message"].GetString();
                ui->submitError->setText(QString::fromStdString(message));
                return false;
            }
        }
    }
   ui->submitError->setText("Unknow error");
    return false;
}
void OnlineWindow::getHistory(){
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type","get_history", allocator);
    std::string name = Session::getInstance()->getUsername();
    document.AddMember("username",tojsonValueType(name,allocator) , allocator);
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
        if (doc.HasMember("history") && doc["history"].IsArray()) {
            const rapidjson::Value& dataArray = doc["history"].GetArray();

            for (rapidjson::SizeType i = this->history.size(); i < dataArray.Size(); ++i) {
                const rapidjson::Value& object = dataArray[i];
                History* history = new History();
                if (object.IsObject()) {
                    if (object.HasMember("room_id") && object["room_id"].IsString()) {
                        const std::string id = object["room_id"].GetString();
                        history->setLevel(id);
                    }

                    if (object.HasMember("start_time") && object["start_time"].IsInt64()) {
                        const long int time= object["start_time"].GetInt64();
                        time_t timestamp = static_cast<time_t>(time);
                        std::string formattedTime;
                        formattedTime.resize(100); // Reserve space for the formatted time
                        std::strftime(&formattedTime[0], formattedTime.size(), "%Y-%m-%d %H:%M", std::localtime(&timestamp));
                        formattedTime.resize(std::strlen(formattedTime.c_str())); // Trim the string to fit the actual length
                        history->setTime(formattedTime);
                    }

                    if (object.HasMember("end_time") && object["end_time"].IsInt()) {
                        const long int time= object["end_time"].GetInt64();
                        time_t timestamp = static_cast<time_t>(time);
                        std::string formattedTime;
                        formattedTime.resize(100); // Reserve space for the formatted time
                        std::strftime(&formattedTime[0], formattedTime.size(), "%Y-%m-%d %H:%M", std::localtime(&timestamp));
                        formattedTime.resize(std::strlen(formattedTime.c_str())); // Trim the string to fit the actual length
                        history->setEnd(formattedTime);
                    }

                    if (object.HasMember("points") && object["points"].IsInt()) {
                        const int totalPoints = object["points"].GetInt();
                        history->setScore(totalPoints);
                    }

                }
                this->history.push_back(history);
            }
        }
    }
}
void OnlineWindow::initTable(){
    ui->resultTable->setRowCount(this->history.size());
    int size = history.size();
    for(int i = history.size()-1; i>=0;i--){
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(history[i]->getTime()));
        newItem->setTextAlignment(4);
        ui->resultTable->setItem(size-1-i, 1, newItem);
        newItem = new QTableWidgetItem(QString::fromStdString(history[i]->getLevel()));
        newItem->setTextAlignment(4);
        ui->resultTable->setItem(size-1-i, 0, newItem);
        newItem = new QTableWidgetItem(QString::fromStdString(history[i]->getEnd()));
        newItem->setTextAlignment(4);
        ui->resultTable->setItem(size-1-i, 2, newItem);
        newItem = new QTableWidgetItem(QString::number(history[i]->getScore()));
        newItem->setTextAlignment(4);
        ui->resultTable->setItem(size-1-i, 3, newItem);
    }
}

void OnlineWindow::on_toHomeNav_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void OnlineWindow::onJoinRoom(){
    this->w->showOutBtn();
    ui->stackedWidget->setCurrentIndex(2);
}
