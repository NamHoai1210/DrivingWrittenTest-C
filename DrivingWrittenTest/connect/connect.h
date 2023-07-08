#ifndef CONNECT_H
#define CONNECT_H

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <QDebug>
#include <string>
#define PORT 8080
#define MAX_BUFFER_SIZE 10240

class Connect
{
private:
    Connect();
    int sockfd;
    struct sockaddr_in serv_addr;
private:
    static Connect *instance;
public:
    static Connect* getInstance();
    bool connectToServer();
    void disconnectToServer();
    void sendServer(std::string);
    char* receiveServer();
    char* both(std::string);

};

#endif // CONNECT_H
