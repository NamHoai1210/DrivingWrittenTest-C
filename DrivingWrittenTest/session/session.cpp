#include "session.h"
Session* Session::instance = nullptr;
Session::Session(){
    this->username = "";
}

Session* Session::getInstance(){
    if(instance == nullptr){
        instance = new Session();
    }
    return instance;
}

void Session::saveSession(std::string username){
    this->username = username;
}

std::string Session::getUsername(){
    return this->username;
}

void Session::saveRoom(RoomEntity *room){
    this->currentRoom = room;
}

RoomEntity* Session::getCurrentRoom(){
    return this->currentRoom;
}

int Session::getRankPoint(){
    return this->rankPoint;
}

void Session::saveRankPoint(int rank){
    this->rankPoint = rank;
}
