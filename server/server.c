#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "cJSON.h"

#define MAX_CLIENTS 10
#define MAX_ROOMS 10
#define MAX_STUDENTS 100
#define MAX_RESULTS 100
#define MAX_QUESTION 100
#define DEFAULT_PORT 8080

typedef struct
{
    char name[256];
    char passwork[256];
    int rank_point;
    bool login;
} student_t;

typedef struct
{
    char id[16];
    char name[256];
    // student_t *student[MAX_STUDENTS];
    time_t time_start;
    int duration;
    int total_points;
    // int num_students;
    int rank_point_limit;
} test_room_t;

typedef struct
{
    char name[16];
    char room_id[16];
    int points;
    time_t start_time;
    time_t end_time;
} test_result_t;

static test_room_t rooms[MAX_ROOMS];
static int num_rooms = 0;
pthread_mutex_t room_mutex;

static test_result_t results[MAX_STUDENTS * MAX_ROOMS];
static int num_results = 0;
pthread_mutex_t result_mutex;

static student_t students[MAX_STUDENTS];
static int num_students = 0;
pthread_mutex_t student_mutex;

static cJSON *questions;
static int num_questions = 0;
pthread_mutex_t question_mutex;

static char *generate_room_id()
{
    // Generate a random integer between 0 and 99999
    int random_num = rand() % 100000;

    // Convert the integer to a string
    char *id_str = malloc(6); // Need space for the digits and null terminator
    sprintf(id_str, "%05d", random_num);
    return id_str;
}

static test_room_t *find_room_by_id(char *room_id)
{
    pthread_mutex_lock(&room_mutex);
    for (int i = 0; i < num_rooms; i++)
    {
        if (strcmp(rooms[i].id, room_id) == 0)
        {
            pthread_mutex_unlock(&room_mutex);
            return &rooms[i];
        }
    }
    pthread_mutex_unlock(&room_mutex);
    return NULL;
}
static student_t *find_student_by_name(char *name)
{
    pthread_mutex_lock(&student_mutex);
    for (int i = 0; i < num_students; i++)
    {
        if (strcmp(students[i].name, name) == 0)
        {
            pthread_mutex_unlock(&student_mutex);
            return &students[i];
        }
    }
    pthread_mutex_unlock(&student_mutex);
    return NULL;
}

static test_result_t *find_result_by_ids(char *name, char *room_id)
{
    pthread_mutex_lock(&result_mutex);
    for (int i = 0; i < num_results; i++)
    {
        if (strcmp(results[i].name, name) == 0 &&
            strcmp(results[i].room_id, room_id) == 0)
        {
            pthread_mutex_unlock(&result_mutex);
            return &results[i];
        }
    }
    pthread_mutex_unlock(&result_mutex);
    return NULL;
}

// Helper function to load test rooms and results from a JSON file
void load_data()
{
    cJSON *root;
    cJSON *room;
    cJSON *result;
    cJSON *student;
    cJSON *question;
    FILE *std = fopen("student.json", "r");
    FILE *rs = fopen("result.json", "r");
    FILE *rm = fopen("room.json", "r");
    FILE *qus = fopen("question.json", "r");
    if (std == NULL || rs == NULL || rm == NULL || qus == NULL)
    {
        printf("load data error\n");
        return;
    }
    fseek(std, 0, SEEK_END);
    fseek(rs, 0, SEEK_END);
    fseek(rm, 0, SEEK_END);
    fseek(qus, 0, SEEK_END);
    long std_size = ftell(std);
    long rs_size = ftell(rs);
    long rm_size = ftell(rm);
    long qus_size = ftell(qus);
    fseek(std, 0, SEEK_SET);
    fseek(rs, 0, SEEK_SET);
    fseek(rm, 0, SEEK_SET);
    fseek(qus, 0, SEEK_SET);

    char *std_data = (char *)malloc(std_size + 1);
    char *rs_data = (char *)malloc(rs_size + 1);
    char *rm_data = (char *)malloc(rm_size + 1);
    char *qus_data = (char *)malloc(qus_size + 1);
    fread(std_data, 1, std_size, std);
    fread(rs_data, 1, rs_size, rs);
    fread(rm_data, 1, rm_size, rm);
    fread(qus_data, 1, qus_size, qus);
    fclose(std);
    fclose(rs);
    fclose(rm);
    fclose(qus);
    // parse rooms data
    root = cJSON_Parse(rm_data);
    if (root != NULL)
    {
        num_rooms = cJSON_GetArraySize(root);
        for (int i = 0; i < num_rooms && i < MAX_ROOMS; i++)
        {
            room = cJSON_GetArrayItem(root, i);
            if (room != NULL)
            {
                strncpy(rooms[i].id, cJSON_GetStringValue(cJSON_GetObjectItem(room, "id")), sizeof(rooms[i].id));
                strncpy(rooms[i].name, cJSON_GetStringValue(cJSON_GetObjectItem(room, "name")), sizeof(rooms[i].name));
                rooms[i].time_start = (time_t)cJSON_GetNumberValue(cJSON_GetObjectItem(room, "time_start"));
                rooms[i].duration = cJSON_GetNumberValue(cJSON_GetObjectItem(room, "duration"));
                // rooms[i].num_students = cJSON_GetNumberValue(cJSON_GetObjectItem(room, "num_students"));
                rooms[i].total_points = cJSON_GetNumberValue(cJSON_GetObjectItem(room, "total_points"));
                rooms[i].rank_point_limit = cJSON_GetNumberValue(cJSON_GetObjectItem(room, "rank_point_limit"));
            }
        }
    }
    cJSON_Delete(root);
    free(rm_data);
    // parse results data
    root = cJSON_Parse(rs_data);
    if (root != NULL)
    {
        num_results = cJSON_GetArraySize(root);
        for (int i = 0; i < num_results && i < MAX_RESULTS; i++)
        {
            result = cJSON_GetArrayItem(root, i);
            if (result != NULL)
            {
                strncpy(results[i].name, cJSON_GetStringValue(cJSON_GetObjectItem(result, "username")), sizeof(results[i].name));
                strncpy(results[i].room_id, cJSON_GetStringValue(cJSON_GetObjectItem(result, "room_id")), sizeof(results[i].room_id));
                results[i].points = cJSON_GetNumberValue(cJSON_GetObjectItem(result, "points"));
                results[i].start_time = (time_t)cJSON_GetNumberValue(cJSON_GetObjectItem(result, "start_time"));
                results[i].end_time = (time_t)cJSON_GetNumberValue(cJSON_GetObjectItem(result, "end_time"));
            }
        }
    }
    cJSON_Delete(root);
    free(rs_data);

    // parse student data
    root = cJSON_Parse(std_data);
    if (root != NULL)
    {
        num_students = cJSON_GetArraySize(root);
        for (int i = 0; i < num_students && i < MAX_STUDENTS; i++)
        {
            student = cJSON_GetArrayItem(root, i);
            if (student != NULL)
            {
                strncpy(students[i].name, cJSON_GetStringValue(cJSON_GetObjectItem(student, "username")), sizeof(students[i].name));
                strncpy(students[i].passwork, cJSON_GetStringValue(cJSON_GetObjectItem(student, "passwork")), sizeof(students[i].passwork));
                students[i].rank_point = cJSON_GetNumberValue(cJSON_GetObjectItem(student, "rank_point")), sizeof(students[i].rank_point);
            }
        }
    }
    cJSON_Delete(root);
    free(std_data);

    // parse question data

    root = cJSON_Parse(qus_data);
    if (root != NULL)
    {
        num_questions = cJSON_GetArraySize(root);
        questions = cJSON_Duplicate(root, true);
    }
    cJSON_Delete(root);
    free(qus_data);
}

// Helper function to save test rooms and results to a JSON file
void save_data()
{
    cJSON *rooms_array = cJSON_CreateArray();
    cJSON *results_array = cJSON_CreateArray();
    cJSON *students_array = cJSON_CreateArray();
    cJSON *room;
    cJSON *result;
    cJSON *student;

    // write room
    pthread_mutex_lock(&room_mutex);
    for (int i = 0; i < num_rooms; i++)
    {
        room = cJSON_CreateObject();
        cJSON_AddStringToObject(room, "id", rooms[i].id);
        cJSON_AddStringToObject(room, "name", rooms[i].name);
        cJSON_AddNumberToObject(room, "time_start", rooms[i].time_start);
        cJSON_AddNumberToObject(room, "duration", rooms[i].duration);
        // cJSON_AddNumberToObject(room, "num_students", rooms[i].num_students);
        cJSON_AddNumberToObject(room, "total_points", rooms[i].total_points);
        cJSON_AddNumberToObject(room, "rank_point_limit", rooms[i].rank_point_limit);
        cJSON_AddItemToArray(rooms_array, room);
    }
    pthread_mutex_unlock(&room_mutex);
    char *data = cJSON_Print(rooms_array);
    FILE *rm = fopen("room.json", "w");
    if (rm != NULL)
    {
        fputs(data, rm);
        fclose(rm);
    }
    free(data);

    // write results
    pthread_mutex_lock(&result_mutex);
    for (int i = 0; i < num_results; i++)
    {
        result = cJSON_CreateObject();
        cJSON_AddStringToObject(result, "username", results[i].name);
        cJSON_AddStringToObject(result, "room_id", results[i].room_id);
        cJSON_AddNumberToObject(result, "points", results[i].points);
        cJSON_AddNumberToObject(result, "start_time", results[i].start_time);
        cJSON_AddNumberToObject(result, "end_time", results[i].end_time);
        cJSON_AddItemToArray(results_array, result);
    }
    pthread_mutex_unlock(&result_mutex);

    data = cJSON_Print(results_array);
    FILE *rs = fopen("result.json", "w");
    if (rm != NULL)
    {
        fputs(data, rs);
        fclose(rs);
    }
    free(data);

    // write student
    pthread_mutex_lock(&student_mutex);
    for (int i = 0; i < num_students; i++)
    {
        student = cJSON_CreateObject();
        cJSON_AddStringToObject(student, "username", students[i].name);
        cJSON_AddStringToObject(student, "passwork", students[i].passwork);
        cJSON_AddNumberToObject(student, "rank_point", students[i].rank_point);
        cJSON_AddItemToArray(students_array, student);
    }
    pthread_mutex_unlock(&student_mutex);
    data = cJSON_Print(students_array);
    FILE *std = fopen("student.json", "w");
    if (std != NULL)
    {
        fputs(data, std);
        fclose(std);
    }
    free(data);
}

cJSON *random_question(int num_required)
{

    cJSON *randomQuestions = cJSON_CreateArray();
    pthread_mutex_lock(&question_mutex);
    bool check_dup[num_questions];
    memset(check_dup, false, num_questions * sizeof(bool));

    int num = 0;
    int randomIndex;
    srand(time(NULL));
    while (num < num_required)
    {
        randomIndex = rand() % num_questions;
        if (check_dup[randomIndex] == false)
        {
            cJSON *item = cJSON_Duplicate(cJSON_GetArrayItem(questions, randomIndex), true);
            cJSON_AddItemToArray(randomQuestions, item);
            check_dup[randomIndex] = true;
            num++;
        }
    }
    pthread_mutex_unlock(&question_mutex);
    return randomQuestions;
}

static void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    cJSON *request;
    cJSON *response;
    char request_buf[1024];
    // login and sign up
    while (1)
    {
        // Receive request from client

        memset(request_buf, 0, sizeof(request_buf));
        int bytes_received = recv(client_socket, request_buf, sizeof(request_buf), 0);
        if (bytes_received < 0)
        {
            perror("recv");
            close(client_socket);
            return NULL;
        }
        printf("%s\n", request_buf);
        // Parse request as JSON
        request = cJSON_Parse(request_buf);
        if (request == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
            }
            close(client_socket);
            return NULL;
        }

        // Process request and construct response
        char *type = cJSON_GetStringValue(cJSON_GetObjectItem(request, "type"));
        response = cJSON_CreateObject();
        if (strcmp(type, "login") == 0)
        {
            // Check login credentials and create new thread for successful logins
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "username"));
            char *password = cJSON_GetStringValue(cJSON_GetObjectItem(request, "password"));
            student_t *std = find_student_by_name(name);
            pthread_mutex_lock(&student_mutex);
            if (std != NULL && strcmp(password, std->passwork) == 0)
            {
                cJSON_AddStringToObject(response, "status", "success");
                cJSON_AddStringToObject(response, "message", "Login complete");
                char *response_str = cJSON_Print(response);
                send(client_socket, response_str, strlen(response_str), 0);
                cJSON_Delete(request);
                cJSON_Delete(response);
                pthread_mutex_unlock(&student_mutex);
                break;
            }
            else
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Invalid username or password");
            }
            pthread_mutex_unlock(&student_mutex);
        }
        else if (strcmp(type, "signup") == 0)
        {
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "username"));
            char *password = cJSON_GetStringValue(cJSON_GetObjectItem(request, "password"));
            student_t *student = find_student_by_name(name);

            pthread_mutex_lock(&student_mutex);
            if (student == NULL && num_students < MAX_STUDENTS)
            {

                student_t *new_student = &students[num_students];
                num_students++;
                strncpy(new_student->name, name, sizeof(new_student->name));
                strncpy(new_student->passwork, password, sizeof(new_student->passwork));

                new_student->rank_point = 0;

                cJSON_AddStringToObject(response, "status", "success");
                cJSON_AddStringToObject(response, "message", "Sign up complete");
            }
            else
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Username is in used");
            }
            pthread_mutex_unlock(&student_mutex);
            save_data();
        }
        else
        {
            cJSON_AddStringToObject(response, "status", "error");
            cJSON_AddStringToObject(response, "message", "Unknown request type");
        }
        char *response_str = cJSON_Print(response);
        send(client_socket, response_str, strlen(response_str), 0);
        cJSON_Delete(request);
        cJSON_Delete(response);
    }

    // use case
    while (1)
    {
        // Receive request from client
        memset(request_buf, 0, sizeof(request_buf));

        int bytes_received = recv(client_socket, request_buf, sizeof(request_buf), 0);
        if (bytes_received < 0)
        {
            perror("recv");
            close(client_socket);
            return NULL;
        }

        // Parse request as JSON
        printf("%s\n", request_buf);
        request = cJSON_Parse(request_buf);
        if (request == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                fprintf(stderr, "Error parsing JSON: %s\n", error_ptr);
            }
            close(client_socket);
            return NULL;
        }

        // Process request and construct response
        response = cJSON_CreateObject();
        char *type = cJSON_GetStringValue(cJSON_GetObjectItem(request, "type"));
        if (type == NULL)
        {
            cJSON_AddStringToObject(response, "status", "error");
            cJSON_AddStringToObject(response, "message", "Missing request type");
        }
        else if (strcmp(type, "create_room") == 0)
        {
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "name"));
            int duration = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "duration"));
            int total_points = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "total_points"));
            int rank_limit = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "rank_point_limit"));
            time_t start = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "time_start"));
            pthread_mutex_lock(&room_mutex);
            if (name != NULL && duration > 0 && total_points > 0 && num_rooms < MAX_ROOMS)
            {
                test_room_t *room = &rooms[num_rooms];
                char *rmid = generate_room_id();
                strncpy(room->id, rmid, sizeof(room->id));
                free(rmid);
                strncpy(room->name, name, sizeof(room->name));
                room->time_start = start;
                room->duration = duration;
                room->total_points = total_points;
                // room->num_students = 0;
                num_rooms++;
                cJSON_AddStringToObject(response, "status", "success");
                cJSON_AddStringToObject(response, "room_id", room->id);
            }
            else
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Invalid parameters or too many rooms");
            }
            pthread_mutex_unlock(&room_mutex);
            save_data();
        }
        else if (strcmp(type, "get_test_summary") == 0)
        {
            char *room_id = cJSON_GetStringValue(cJSON_GetObjectItem(request, "room_id"));
            test_room_t *room = find_room_by_id(room_id);
            if (room != NULL)
            {
                pthread_mutex_lock(&room_mutex);
                cJSON_AddStringToObject(response, "status", "success");
                cJSON_AddStringToObject(response, "id", room->id);
                cJSON_AddStringToObject(response, "name", room->name);
                cJSON_AddNumberToObject(response, "time_start", (double)room->time_start);
                cJSON_AddNumberToObject(response, "duration", room->duration);
                cJSON_AddNumberToObject(response, "total_points", room->total_points);
                cJSON_AddNumberToObject(response, "rank_point_limit", room->rank_point_limit);
                pthread_mutex_unlock(&room_mutex);

                // Add the students to the cJSON object as sub-objects
                cJSON *students_array = cJSON_CreateArray();
                pthread_mutex_lock(&result_mutex);
                for (int i = 0; i < num_results; i++)
                {
                    if (strcmp(results[i].room_id, room_id) == 0)
                    {
                        cJSON *item = cJSON_CreateObject();
                        cJSON_AddStringToObject(item, "username", results[i].name);
                        cJSON_AddNumberToObject(item, "points", results[i].points);
                        cJSON_AddNumberToObject(item, "start_time", results[i].start_time);
                        cJSON_AddNumberToObject(item, "end_time", results[i].end_time);
                        cJSON_AddItemToArray(students_array, item);
                    }
                }
                pthread_mutex_unlock(&result_mutex);
                cJSON_AddItemToObject(response, "students", students_array);
            }
            else
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Invalid room ID");
            }
        }
        else if (strcmp(type, "join_room") == 0)
        {
            char *room_id = cJSON_GetStringValue(cJSON_GetObjectItem(request, "room_id"));
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "username"));
            test_room_t *room = find_room_by_id(room_id);
            student_t *student = find_student_by_name(name);
            pthread_mutex_lock(&student_mutex);
            pthread_mutex_lock(&room_mutex);
            if (room == NULL || student == NULL)
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "room_id or username not exist");
            }
            else if (student->rank_point < room->rank_point_limit)
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Rank Point not over room limit");
            }
            else
            {
                cJSON_AddStringToObject(response, "status", "success");
                cJSON_AddStringToObject(response, "message", "Successfully joined room");

                cJSON_AddStringToObject(response, "id", room->id);
                cJSON_AddStringToObject(response, "name", room->name);
                cJSON_AddNumberToObject(response, "time_start", (double)room->time_start);
                cJSON_AddNumberToObject(response, "duration", room->duration);
                cJSON_AddNumberToObject(response, "total_points", room->total_points);
                cJSON_AddNumberToObject(response, "rank_point_limit", room->rank_point_limit);
            }
            pthread_mutex_unlock(&student_mutex);
            pthread_mutex_unlock(&room_mutex);
        }
        else if (strcmp(type, "start_test") == 0)
        {
            char *room_id = cJSON_GetStringValue(cJSON_GetObjectItem(request, "room_id"));
            test_room_t *room = find_room_by_id(room_id);

            int point;
            if (room != NULL)
            {
                pthread_mutex_lock(&room_mutex);
                point = room->total_points;
                pthread_mutex_unlock(&room_mutex);

                char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "username"));
                test_result_t *result = find_result_by_ids(name, room_id);
                pthread_mutex_lock(&result_mutex);
                if (result == NULL)
                {
                    result = &results[num_results];
                    strncpy(result->name, name, sizeof(result->name));
                    strncpy(result->room_id, room_id, sizeof(result->room_id));
                    result->points = 0;
                    result->start_time = time(NULL);
                    num_results++;
                }
                pthread_mutex_unlock(&result_mutex);
                save_data();
                cJSON_AddStringToObject(response, "status", "success");
                cJSON *arr = random_question(point);
                cJSON_AddItemToObject(response, "questions", arr);
            }
            else
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Invalid room ID");
            }
        }
        else if (strcmp(type, "submit_answer") == 0)
        {
            char *room_id = cJSON_GetStringValue(cJSON_GetObjectItem(request, "room_id"));
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "username"));
            // test_room_t *room = find_room_by_id(room_id);
            test_result_t *result = find_result_by_ids(name, room_id);
            if (result == NULL)
            {
                cJSON_AddStringToObject(response, "status", "error");
                cJSON_AddStringToObject(response, "message", "Invalid room ID or student ID");
            }
            else
            {
                student_t *student = find_student_by_name(name);
                int points = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "points"));
                pthread_mutex_lock(&student_mutex);
                student->rank_point += points;
                pthread_mutex_unlock(&student_mutex);

                time_t start_time = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "start_time"));
                time_t end_time = cJSON_GetNumberValue(cJSON_GetObjectItem(request, "end_time"));
                pthread_mutex_lock(&result_mutex);
                result->points = points;
                result->start_time = start_time;
                result->end_time = end_time;
                pthread_mutex_unlock(&result_mutex);

                cJSON_AddStringToObject(response, "status", "success");
                cJSON_AddStringToObject(response, "message", "Answer submitted successfully");
                save_data();
            }
        }
        else if (strcmp(type, "get_history") == 0)
        {
            char *name = cJSON_GetStringValue(cJSON_GetObjectItem(request, "username"));
            cJSON *history = cJSON_CreateArray();
            pthread_mutex_lock(&result_mutex);
            for (int i = 0; i < num_results; i++)
            {
                if (strcmp(results[i].name, name) == 0)
                {
                    cJSON *item = cJSON_CreateObject();
                    cJSON_AddStringToObject(item, "room_id", results[i].room_id);
                    cJSON_AddNumberToObject(item, "points", results[i].points);
                    cJSON_AddNumberToObject(item, "start_time", results[i].start_time);
                    cJSON_AddNumberToObject(item, "end_time", results[i].end_time);
                    cJSON_AddItemToArray(history, item);
                }
            }
            pthread_mutex_unlock(&result_mutex);
            cJSON_AddStringToObject(response, "status", "success");
            cJSON_AddItemToObject(response, "history", history);
        }
        else if (strcmp(type, "get_all_rooms") == 0)
        {
            cJSON *all_room = cJSON_CreateArray();
            pthread_mutex_lock(&room_mutex);
            for (int i = 0; i < num_rooms; i++)
            {
                cJSON *item = cJSON_CreateObject();
                cJSON_AddStringToObject(item, "id", rooms[i].id);
                cJSON_AddStringToObject(item, "name", rooms[i].name);
                cJSON_AddNumberToObject(item, "time_start", rooms[i].time_start);
                cJSON_AddNumberToObject(item, "duration", rooms[i].duration);
                cJSON_AddNumberToObject(item, "total_points", rooms[i].total_points);
                cJSON_AddNumberToObject(item, "rank_point_limit", rooms[i].rank_point_limit);
                cJSON_AddItemToArray(all_room, item);
            }
            pthread_mutex_unlock(&room_mutex);
            cJSON_AddStringToObject(response, "status", "success");
            cJSON_AddItemToObject(response, "rooms", all_room);
        }
        else
        {
            cJSON_AddStringToObject(response, "status", "error");
            cJSON_AddStringToObject(response, "message", "Unknown request type");
        }

        // Serialize response as JSON and send to client
        char *response_str = cJSON_PrintUnformatted(response);
        send(client_socket, response_str, strlen(response_str), 0);
        cJSON_Delete(request);
        cJSON_Delete(response);
    }
    // Clean up and close socket
    close(client_socket);
    return NULL;
}

int main()
{
    // Initialize socket
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int address_len = sizeof(server_address);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(DEFAULT_PORT);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    load_data();
    for (int i = 0; i < num_results; i++)
    {
        printf("%s\n", results[i].room_id);
        printf("%s\n", results[i].name);
        printf("%i\n\n", results[i].points);
    }
    //  Handle incoming connections in a loop
    while (1)
    {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t *)&address_len);
        if (client_socket < 0)
        {
            perror("accept");
            continue;
        }
        pthread_t thread;
        if (pthread_create(&thread, NULL, &handle_client, &client_socket) != 0)
        {
            perror("pthread_create");
            close(client_socket);
            continue;
        }
    }

    // Close server socket
    close(server_socket);
    save_data();
    return 0;
}