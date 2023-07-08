#include "connect.h"
Connect* Connect::instance = nullptr;
Connect::Connect()
{
    this->sockfd = 0;
}
Connect* Connect::getInstance(){
    if(instance == nullptr){
        instance = new Connect();
    }
    return instance;
}
bool Connect::connectToServer(){
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        qDebug() << "\n Socket creation error \n";
        return false;
    }

    memset(&(this->serv_addr), '0', sizeof(this->serv_addr));

    this->serv_addr.sin_family = AF_INET;
    this->serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &(this->serv_addr).sin_addr) <= 0)
    {
        qDebug() << "\nInvalid address/ Address not supported \n";
        return false;
    }

    if (connect(sockfd, (struct sockaddr *)&(this->serv_addr), sizeof(this->serv_addr)) < 0)
    {
        qDebug() << "\nConnection Failed \n";
        return false;
    }
    return true;
}
void Connect::disconnectToServer(){
    close(sockfd);
}
void Connect::sendServer(std::string str){
    char* message = const_cast<char*>(str.c_str());
    send(sockfd, message, strlen(message), 0);
}
char* Connect::receiveServer(){
    char buffer[MAX_BUFFER_SIZE] = {0};
    int valread = read(sockfd, buffer, MAX_BUFFER_SIZE);
    if (valread <= 0)
    {
        return "";
    }
    char *message = (char*)malloc(valread + 1);
    strncpy(message, buffer, valread);
    message[valread] = '\0';
    return message;
}
char* Connect::both(std::string str){
    char* message = const_cast<char*>(str.c_str());
    this->sendServer(message);
    return this->receiveServer();
}
