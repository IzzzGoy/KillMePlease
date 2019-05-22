#ifndef SERVER_H
#define SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#include<chrono>
#include<pthread.h>
#include<thread>
#include<vector>
#include<unistd.h>
#include <fcntl.h>

#include"player.h"
#include"semafor.h"
#include"bot.h"

enum State
{
    START = 0,
    WAITING,
    STARTGAME,
    RESULTS
};

struct Scores
{
    std::vector<unsigned short*> scores;
    Scores() {}
};

struct Coordinates
{
    std::vector<double*> X;
    std::vector<double*> Y;
    short* grid;
    Coordinates()
    {
        X.resize(4);
        Y.resize(4);
    }
    void set_grid(short* grid)
    {
        this->grid = grid;
    }
    void set(int i,double* x,double* y)
    {
        X[i] = x;
        Y[i] = y;
    }
};

struct PlayerInfo
{
    Scores* scores;
    State* state;
    Player* player;
    Coordinates* coordinates;
    PlayerInfo(State* state,Player* player,Coordinates* coordinates,Scores* scores)
    {
        this->coordinates = coordinates;
        this->player = player;
        this->state = state;
        this->scores = scores;
    }
};

struct BotInfo
{
    State* state;
    Bot* bot;
    BotInfo(Bot* bot,State* state)
    {
        this->bot = bot;
        this->state = state;
    }
};

class Server
{
private:
    Scores scores;
    pthread_t protocolThread;
    Semafor semafor;
    size_t numbOfPlayers;
    size_t numbOfBots;
    Coordinates coordinates;
    std::vector<pthread_t> threads;
    std::vector<Player*> players;
    std::vector<Bot*> bots;
    std::vector<PlayerInfo*> playersInfo;
    std::vector<BotInfo*> BotsInfo;

    short grid[400] ={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
                      -1, 3, 3, 3, 3, 3, 3, 3, 3,-1,-1, 3, 3, 3, 3, 3, 3, 3, 3,-1,
                      -1, 3,-1, 3, 3,-1,-1,-1, 3, 3, 3, 3,-1,-1,-1, 3, 3,-1, 3,-1,
                      -1, 3,-1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,-1, 3,-1,
                      -1, 3,-1,-1,-1, 3, 3,-1,-1, 3, 3,-1,-1, 3, 3,-1,-1,-1, 3,-1,
                      -1, 3, 3, 3, 3, 3, 3,-1,-1, 3, 3,-1,-1, 3, 3, 3, 3, 3, 3,-1,
                      -1,-1,-1,-1,-1, 3, 3,-1,-1, 3, 3,-1,-1, 3, 3,-1,-1,-1,-1,-1,
                      -1, 3, 3, 3,-1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,-1, 3, 3, 3,-1,
                      -1,-1, 3,-1,-1, 3, 3,-1,-1, 3, 3,-1,-1, 3, 3,-1,-1, 3,-1,-1,
                       3, 3, 3, 3, 3, 3, 3,-1, 3, 3, 3, 3,-1, 3, 3, 3, 3, 3, 3, 3,
                       3, 3, 3, 3, 3, 3, 3,-1, 3, 3, 3, 3,-1, 3, 3, 3, 3, 3, 3, 3,
                      -1,-1, 3,-1,-1, 3, 3,-1,-1,-1,-1,-1,-1, 3, 3,-1,-1, 3,-1,-1,
                      -1, 3, 3, 3,-1,-1, 3, 3, 3, 3, 3, 3, 3, 3,-1,-1, 3, 3, 3,-1,
                      -1,-1,-1,-1,-1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,-1,-1,-1,-1,-1,
                      -1, 3, 3, 3, 3, 3, 3,-1,-1,-1,-1,-1,-1, 3, 3, 3, 3, 3, 3,-1,
                      -1, 3, 3, 3,-1,-1, 3, 3, 3,-1,-1, 3, 3, 3,-1,-1, 3, 3, 3,-1,
                      -1,-1,-1, 3, 3, 3, 3,-1, 3, 3, 3, 3,-1, 3, 3, 3, 3,-1,-1,-1,
                      -1, 3, 3, 3,-1,-1,-1,-1,-1, 3, 3,-1,-1,-1,-1,-1, 3, 3, 3,-1,
                      -1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,-1,
                      -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int socketServer;
    struct sockaddr_in addr;
public:
    State state;
    Server();
    ~Server();
    void serverInit(int numb);
    void doServer();
    static void* playerServis(void* arg);
    static void* botServis(void* arg);
};

#endif // SERVER_H
