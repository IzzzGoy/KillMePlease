#include "table.h"
#include <iostream>
Table::~Table()
{
    for(size_t i = 0; table.size(); i++)
    {
        delete table[i];
    }
    for(size_t i = 0; i < players.size(); i++)
    {
        delete players[i];
    }
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
}

void Table::Drow(short int* grid,vector<double*> X,vector<double*> Y, QGraphicsScene *scene)
{
//    for(size_t i = 0; table.size(); i++)
//    {
//        table[i]->
//    }
//    for(size_t i = 0; i < players.size(); i++)
//    {
//        delete players[i];
//    }
//    std::cout << "Scene in table address: "<<scene;
//    scene->clear();
//    scene = new QGraphicsScene;
//    std::cout << "Scene in table address: "<<scene;
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
    players[0] = scene->addPixmap(GG);
    players[1] = scene->addPixmap(pinky);
    players[2] = scene->addPixmap(blu);
    players[3] = scene->addPixmap(grey);

    for(size_t i = 0;i < 4;i++)
    {
        players[i]->setPos(*Y[i] * 20,*X[i] * 20);
    }
}
