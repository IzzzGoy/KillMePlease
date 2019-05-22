#ifndef CLIENT_H
#define CLIENT_H

#include<stdio.h>
#include<stdlib.h>
#include<chrono>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<thread>
#include<vector>
#include<unistd.h>
#include<fcntl.h>
#include<iostream>

#include"server.h"

class Client
{
private:
    int socketClient;
    struct sockaddr_in addr;
    char direction;

public:
    short grid[400];
    std::vector<double> frameX;
    std::vector<double> frameY;
    Scores scores;
    int id;
    unsigned short score;
    Coordinates coordinates;
    Client();
    ~Client();
    void start_client(char* address);
    void close_client();
    bool protocol();
    void set_direction(char direction);
};

#endif // CLIENT_H
