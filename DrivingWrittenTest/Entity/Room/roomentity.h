#ifndef ROOMENTITY_H
#define ROOMENTITY_H

#include<string>
#include <ctime>
class RoomEntity
{
public:
    RoomEntity();
    RoomEntity(std::string,std::string,std::time_t, int,int,int);
    std::string getId() const;
    void setId(const std::string &newId);
    std::string getName() const;
    void setName(const std::string &newName);
    std::time_t getTimeStart() const;
    void setTimeStart(std::time_t newTimeStart);
    int getDuration() const;
    void setDuration(int newDuration);
    int getTotalPoints() const;
    void setTotalPoints(int newTotalPoints);
    int getRankPointLimit() const;
    void setRankPointLimit(int newRankPointLimit);
std::string getLevel() const;
    void setLevel(const std::string &newName);

private:
    std::string id;
    std::string name;
    std::time_t timeStart;
    int duration;
    int totalPoints;
    int rankPointLimit;
    std::string level;

};

#endif // ROOMENTITY_H
