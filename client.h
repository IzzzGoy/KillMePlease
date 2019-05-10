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

#include"server.h"

/*struct Coordinates
{
    std::vector<double*> X;
    std::vector<double*> Y;
    short* grid;
    Coordinates(){}
};*/


class Client
{
private:
    int socketClient;
    struct sockaddr_in addr;
    char direction;

public:
    unsigned short score;
    Coordinates coordinates;
    Client();
    ~Client();
    void start_client(char* address);
    void close_client();
    void protocol();
    void set_direction(char direction);
};

#endif // CLIENT_H
