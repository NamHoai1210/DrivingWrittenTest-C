#ifndef SESSION_H
#define SESSION_H
#include <string>


class Session
{
private:
    static Session* instance;
    std::string username;
    Session();
public:
    static Session* getInstance();
    void saveSession(std::string);
    std::string getUsername();
};

#endif // SESSION_H
