#ifndef HISTORY_H
#define HISTORY_H
#include <string>

class History
{
public:
    History();
    History(std::string,std::string,std::string,float);
    std::string getTime() const;
    void setTime(const std::string &newTime);
    std::string getLevel() const;
    void setLevel(const std::string &newLevel);
    std::string getDuration() const;
    void setDuration(const std::string &newDuration);
    float getScore() const;
    void setScore(float newScore);
    std::string getEnd() const;
    void setEnd(const std::string &newTime);

private:
    std::string time;
    std::string level;
    std::string duration;
    std::string end;
    float score;

};

#endif // HISTORY_H
