#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "cJSON.h"

#define PORT 8080
#define MAX_BUFFER_SIZE 10240

void send_message(int sockfd, const char *message)
{
    send(sockfd, message, strlen(message), 0);
}

char *receive_message(int sockfd)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    int valread = read(sockfd, buffer, MAX_BUFFER_SIZE);
    if (valread <= 0)
    {
        return NULL;
    }
    char *message = malloc(valread + 1);
    strncpy(message, buffer, valread);
    message[valread] = '\0';
    return message;
}

int main(int argc, char const *argv[])
{
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    char buffer[MAX_BUFFER_SIZE];
    cJSON *root, *item;

    int choice;
    int login = 0;
    char name[30];
    char pass[256];
    while (1)
    {
        printf("\nMenu\n");
        printf("---------------------\n");
        printf("1.login\n");
        printf("2.signup\n");
        scanf("%i", &choice);

        root = cJSON_CreateObject();
        switch (choice)
        {
        case 1:
            printf("username: ");
            scanf(" %s", name);

            printf("password: ");
            scanf(" %s", pass);
            cJSON_AddStringToObject(root, "type", "login");
            cJSON_AddStringToObject(root, "username", name);
            cJSON_AddStringToObject(root, "password", pass);
            login = 1;
            break;
        case 2:
            printf("username: ");
            scanf(" %s", name);
            printf("password: ");
            scanf(" %s", pass);
            cJSON_AddStringToObject(root, "type", "signup");
            cJSON_AddStringToObject(root, "username", name);
            cJSON_AddStringToObject(root, "password", pass);
            break;
        }

        sprintf(buffer, "%s\n", cJSON_Print(root));
        send_message(sockfd, buffer);
        char *mess = receive_message(sockfd);
        printf("%s", mess);
        free(mess);
        cJSON_Delete(root);
        if (login == 1)
        {
            break;
        }
    }
    char room_id[30];
    while (1)
    {
        printf("\nMenu\n");
        printf("---------------------\n");
        printf("1.create room\n");
        printf("2.join room\n");
        printf("3.start test\n");
        printf("4.end test\n");
        printf("5.get results\n");
        printf("6.get room summary\n");
        printf("7.get available room\n\n");
        scanf("%i", &choice);

        root = cJSON_CreateObject();
        switch (choice)
        {
        case 1:
            cJSON_AddStringToObject(root, "type", "create_room");
            cJSON_AddStringToObject(root, "name", "room2");
            cJSON_AddNumberToObject(root, "duration", 45);
            cJSON_AddNumberToObject(root, "total_points", 15);
            cJSON_AddNumberToObject(root, "time_start", time(NULL));
            cJSON_AddNumberToObject(root, "rank_point_limit", 110);
            break;
        case 2:
            cJSON_AddStringToObject(root, "type", "join_room");
            cJSON_AddStringToObject(root, "room_id", "room1");
            cJSON_AddStringToObject(root, "username", name);
            break;
        case 3:
            cJSON_AddStringToObject(root, "type", "start_test");
            cJSON_AddStringToObject(root, "room_id", "room1");
            cJSON_AddStringToObject(root, "username", name);
            break;
        case 4:
            cJSON_AddStringToObject(root, "type", "submit_answer");
            cJSON_AddStringToObject(root, "room_id", "room1");
            cJSON_AddStringToObject(root, "username", name);
            cJSON_AddNumberToObject(root, "points", 30);
            cJSON_AddNumberToObject(root, "start_time", time(NULL));
            cJSON_AddNumberToObject(root, "end_time", time(NULL));
            break;
        case 5:
            cJSON_AddStringToObject(root, "type", "get_history");
            cJSON_AddStringToObject(root, "username", name);
            break;
        case 6:
            cJSON_AddStringToObject(root, "type", "get_test_summary");
            cJSON_AddStringToObject(root, "room_id", "room1");
            break;
        case 7:
            cJSON_AddStringToObject(root, "type", "get_all_rooms");
            cJSON_AddStringToObject(root, "username", name);
            break;
        }
        sprintf(buffer, "%s\n", cJSON_Print(root));
        printf("%s\n", buffer);
        send_message(sockfd, buffer);
        char *mess = receive_message(sockfd);
        printf("%s", mess);
        free(mess);
        cJSON_Delete(root);
    }
    close(sockfd);
    return 0;
}