#include "user.h"

User::User()
{

}
User::User(std::string username, std::string password){
    this->username = username;
    this->password = password;
}
std::string User::getUsername(){
    return this->username;
}
std::string User::getPassword(){
    return this->password;
}
void User::setUsername(std::string username){
    this->username = username;
}
void User::setPassword(std::string password){
    this->password = password;
}

std::string User::toJsonString(std::string type){
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.AddMember("type",tojsonValueType(type,allocator), allocator);
    document.AddMember("username",tojsonValueType(username,allocator) , allocator);
    document.AddMember("password", tojsonValueType(password,allocator), allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    std::string jsonString = buffer.GetString();

    return jsonString;
}
