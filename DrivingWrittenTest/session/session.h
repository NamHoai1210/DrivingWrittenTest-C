#ifndef SESSION_H
#define SESSION_H
#include <string>
#include "../Entity/Room/roomentity.h"

class Session
{
private:
    static Session* instance;
    std::string username;
    int rankPoint;
    RoomEntity* currentRoom;
    Session();
public:
    static Session* getInstance();
    void saveSession(std::string);
    std::string getUsername();
    void saveRoom(RoomEntity*);
    RoomEntity* getCurrentRoom();
    void saveRankPoint(int);
    int getRankPoint();
};

#endif // SESSION_H
