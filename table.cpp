#include "table.h"
#include <iostream>
Table::~Table()
{
}

Table::Table()
{
    food.load(":/res/food.png");
    no_food.load(":/res/no_food.png");
    wall.load(":/res/wall.png");
    GG.load(":/res/GG.png");
    grey.load(":/res/grey.png");
    pinky.load(":/res/pinky.png");
    blu.load(":/res/blu.png");
    players.resize(4);
    table.resize(400);
    playersTexture.resize(3);

}

void Table::Drow(short int* grid,vector<double> X,vector<double> Y, QGraphicsScene *scene,int id)
{
    for(size_t x = 0; x < 20; x++)
    {
        for(size_t y = 0; y < 20; y++)
        {
            if(grid[x + 20 * y] == -1)
            {
                QGraphicsPixmapItem* tmp;
                tmp = scene->addPixmap(wall);
                tmp->setPos(x*20,y*20);
                //table[x + 20 * y] = tmp;
            }
            else if(grid[x + 20 * y] == 0)
            {
                QGraphicsPixmapItem* tmp;
                tmp = scene->addPixmap(no_food);
                tmp->setPos(x*20,y*20);
                //table[x + 20 * y] = tmp;
            }
            else if(grid[x + 20 * y] == 3)
            {
                QGraphicsPixmapItem* tmp;
                tmp = scene->addPixmap(food);
                tmp->setPos(x*20,y*20);
                //table[x + 20 * y] = tmp;
            }
        }
    }

    players[id] = scene->addPixmap(GG);
    playersTexture[0] = pinky;
    playersTexture[1] = blu;
    playersTexture[2] = grey;

    for(size_t i = 0,j = 0; i < 4; i++ )
    {
        if(i != id)
        {
            players[i] = scene->addPixmap(playersTexture[j]);
            j++;
        }
    }

    for(size_t i = 0;i < 4;i++)
    {
        players[i]->setPos(Y[i] * 20,X[i] * 20);
    }
}
