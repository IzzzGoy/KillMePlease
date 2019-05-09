#include "bot.h"

unsigned short Bot::get_score()
{
    return score;
}

double Bot::get_speed()
{
    return speed;
}

void Bot::step()
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

void Bot::move()
{
    srand(time(NULL));
    std::vector<char> tmp(0);
    int N = 20;
    switch (direction)
    {
        case 'w':
            if(grid[(X - 1) * N + Y ] != -1 && X != 0)
            {
                X--;
                return;
            }
            else
            {
                if(grid[(X + 1) * N + Y] != -1 && X != 19)
                {
                    tmp.push_back('s');
                }
                else if(grid[X * N + Y - 1] != -1 && Y !=0)
                {
                    tmp.push_back('a');
                }
                else if (grid[X * N + Y +1] != -1 && Y != 19)
                {
                    tmp.push_back('d');
                }
                direction = tmp[rand() % tmp.size()];
                switch (direction)
                {
                    case 's':
                        X++;
                        return;
                        break;
                    case 'a':
                        Y--;
                        return;
                        break;
                    case 'd':
                        Y++;
                        return;
                    break;
                    default:
                        break;
                }
            }

            break;
        case 's':
            if(grid[(X + 1) * N + Y ] != -1 && X != 19)
            {
                X++;
                return;
            }
            else
            {
                if(grid[(X - 1) * N + Y] != -1 && X != 0)
                {
                    tmp.push_back('w');
                }
                else if(grid[X * N + Y - 1] != -1 && Y !=0)
                {
                    tmp.push_back('a');
                }
                else if (grid[X * N + Y + 1] != -1 && Y != 19)
                {
                    tmp.push_back('d');
                }
                direction = tmp[rand() % tmp.size()];
                switch (direction)
                {
                case 'w':
                    X--;
                    return;
                    break;
                case 'a':
                    Y--;
                    return;
                    break;
                case 'd':
                    Y++;
                    return;
                    break;
                default:
                    break;
                }
            }
        case 'a':
            if(grid[X * N + Y - 1 ] != -1 && Y != 0)
            {
                Y--;
                return;
            }
            else
            {
                if(grid[(X - 1) * N + Y] != -1 && X != 0)
                {
                    tmp.push_back('w');
                }
                else if(grid[(X + 1) * N + Y] != -1 && X !=19)
                {
                    tmp.push_back('s');
                }
                else if (grid[X * N + Y + 1] != -1 && Y != 19)
                {
                    tmp.push_back('d');
                }
                direction = tmp[rand() % tmp.size()];
                switch (direction)
                {
                case 'w':
                    X--;
                    return;
                    break;
                case 's':
                    X++;
                    return;
                    break;
                case 'd':
                    Y++;
                    return;
                    break;
                default:
                    break;
                }
            }
    case 'd':
        if(grid[X * N + Y + 1 ] != -1 && Y != 19)
        {
            Y--;
            return;
        }
        else
        {
            if(grid[(X - 1) * N + Y] != -1 && X != 0)
            {
                tmp.push_back('w');
            }
            else if(grid[(X + 1) * N + Y] != -1 && X !=19)
            {
                tmp.push_back('s');
            }
            else if (grid[X * N + Y - 1] != -1 && Y != 0)
            {
                tmp.push_back('d');
            }
            direction = tmp[rand() % tmp.size()];
            switch (direction)
            {
            case 'w':
                X--;
                return;
                break;
            case 's':
                X++;
                return;
                break;
            case 'a':
                Y--;
                return;
                break;
            default:
                break;
            }
        }
    default:
        break;
    }
}

void Bot::eat()
{
    semafor->Stop();
    semafor->Get(X * 20 + Y);
    if(grid[X * 20 + Y] == 3)
    {
        score++;
        grid[X * 20 + Y] = 0;
    }
    semafor->Take(X * 20 + Y);
}

Bot::Bot(short *grid, Semafor *semafor)
{
    this->grid = grid;
    srand(time(NULL));
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
    speed = 0.05;
    int tmp = rand() % 4;
    switch (tmp)
    {
        case 0:
            direction = 'w';
            break;
        case 1:
            direction = 's';
            break;
       case 2:
             direction = 'a';
             break;
       case 3:
            direction = 'd';
            break;
    default:
        break;
    }
    this->semafor = semafor;
}
