#include "roomentity.h"

RoomEntity::RoomEntity()
{
    level = "";
}
RoomEntity::RoomEntity(std::string id,std::string name,std::time_t timeStart, int duration,int totalPoints,int rankPointLimit){
    this->id = id;
    this->name = name;
    this->timeStart = timeStart;
    this->duration = duration;
    this->totalPoints = totalPoints;
    this->rankPointLimit = rankPointLimit;
    this->level = "";
}
std::string RoomEntity::getId() const
{
    return id;
}

void RoomEntity::setId(const std::string &newId)
{
    id = newId;
}

std::string RoomEntity::getName() const
{
    return name;
}

void RoomEntity::setName(const std::string &newName)
{
    name = newName;
}

std::time_t RoomEntity::getTimeStart() const
{
    return timeStart;
}

void RoomEntity::setTimeStart(std::time_t newTimeStart)
{
    timeStart = newTimeStart;
}

int RoomEntity::getDuration() const
{
    return duration;
}

void RoomEntity::setDuration(int newDuration)
{
    duration = newDuration;
}

int RoomEntity::getTotalPoints() const
{
    return totalPoints;
}

void RoomEntity::setTotalPoints(int newTotalPoints)
{
    totalPoints = newTotalPoints;
}

int RoomEntity::getRankPointLimit() const
{
    return rankPointLimit;
}

void RoomEntity::setRankPointLimit(int newRankPointLimit)
{
    rankPointLimit = newRankPointLimit;
}

std::string RoomEntity::getLevel() const
{
    return level;
}

void RoomEntity::setLevel(const std::string &newName)
{
    level = newName;
}
