#ifndef SESSION_H
#define SESSION_H
#include <string>
#include "../Entity/Room/roomentity.h"

class Session
{
private:
    static Session* instance;
    std::string username;
    RoomEntity* currentRoom;
    Session();
public:
    static Session* getInstance();
    void saveSession(std::string);
    std::string getUsername();
    void saveRoom(RoomEntity*);
    RoomEntity* getCurrentRoom();
};

#endif // SESSION_H
