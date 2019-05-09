#ifndef BOT_H
#define BOT_H

#include<ctime>
#include<stdlib.h>

#include"semafor.h"
#include<vector>

class Bot
{
private:
    Semafor* semafor;
    short* grid;
    int X,Y;
    double speed;
    unsigned short score;
    char direction;
public:
    double realX,realY;
    unsigned short get_score();
    double get_speed();
    void step();
    void move();
    void eat();
    Bot(short* grid, Semafor* semafor);
};

#endif // BOT_H
