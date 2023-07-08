#include "history.h"

History::History()
{

}
History::History(std::string time,std::string level,std::string duration,float score = 0){
    this->time = time;
    this->level = level;
    this->duration = duration;
    this-> score =  score;
}

std::string History::getTime() const
{
    return time;
}

void History::setTime(const std::string &newTime)
{
    time = newTime;
}

std::string History::getLevel() const
{
    return level;
}

void History::setLevel(const std::string &newLevel)
{
    level = newLevel;
}

std::string History::getDuration() const
{
    return duration;
}

void History::setDuration(const std::string &newDuration)
{
    duration = newDuration;
}

float History::getScore() const
{
    return score;
}

void History::setScore(float newScore)
{
    score = newScore;
}
