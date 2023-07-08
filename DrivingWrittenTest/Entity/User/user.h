#ifndef USER_H
#define USER_H
#include "../../rapidjson/document.h"
#include "../../rapidjson/istreamwrapper.h"
#include "../../rapidjson/writer.h"
#include "../Utils/utils.h"
#include <string>
class User
{
public:
    User();
    User(std::string, std::string);
    std::string getUsername();
    std::string getPassword();
    void setUsername(std::string);
    void setPassword(std::string);
    std::string toJsonString(std::string type);
private:
    std::string username;
    std::string password;
};

#endif // USER_H
