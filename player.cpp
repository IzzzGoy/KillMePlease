#include "player.h"

void Player::set_direction(char direction)
{
    this->direction = direction;
}

unsigned short Player::get_score()
{
    return score;
}

double Player::get_speed()
{
    return speed;
}

void Player::step()
{
    switch (direction)
    {
    case 'w':
            realX -= speed;
            break;
    case 's':
            realX += speed;
            break;
    case 'a':
            realY -= speed;
            break;
    case 'd':
            realY += speed;
            break;
    default:
        break;
    }
}

bool Player::move()
{
    int N = 20;
    switch (direction)
    {
    case 'w':
            if(grid[(X - 1) * N + Y] != -1 && X != 0)
            {
                X--;
                return true;
            }
            else
            {
                return false;
            }
            break;
    case 's':
            if(grid[(X + 1) * N + Y] != -1 && X != 19)
            {
                X++;
                return true;
            }
            else
            {
                return false;
            }
            break;
    case 'a':
            if(grid[X * N + Y - 1] != -1 && Y != 0)
            {
                Y--;
                return true;
            }
            else
            {
                return false;
            }
            break;
    case 'd':
            if(grid[X * N + Y + 1] != -1 && Y != 19)
            {
                Y++;
                return true;
            }
            else
            {
                return false;
            }
            break;
    default:
        break;
    }
    return false;
}

void Player::eat()
{
    semafor->Take(X * 20 + Y);
    if(grid[X * 20 + Y] == 3)
    {
        score++;
        grid[X * 20 + Y] = 0;
    }
    semafor->Give(X * 20 +Y);
}

Player::Player(int socket, short *grid, int id, Semafor *semafor)
{
    socketPlayer = socket;
    this->grid = grid;
    int tmpX,tmpY;
    do
    {
        tmpX = rand() % 20;
        tmpY = rand() % 20;
    }
    while(grid[20*tmpX + tmpY] != 3);
    X = tmpX;
    Y = tmpY;
    realX = X;
    realY = Y;
    score = 0;
    speed = 0.1;
    direction = 'w';
    this->semafor = semafor;
    ID = id;
}
