#ifndef PLAYER_H
#define PLAYER_H
#include<stdio.h>
#include<stdlib.h>

#include"semafor.h"

class Player
{
private:
    short* grid;
    int X,Y;
    double speed;
    char direction;
    Semafor* semafor;
public:
    unsigned short score;
    int ID;
    int socketPlayer;
    double realX,realY;
    void set_direction(char direction);
    unsigned short get_score();
    double get_speed();
    void step();
    bool move();
    void eat();
    Player(int socket,short* grid,int id,Semafor* semafor);
};

#endif // PLAYER_H
