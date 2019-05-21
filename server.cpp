#include "server.h"
#include<iostream>

Server::Server()
{

}

Server::~Server()
{
    for(size_t i = 0; i < players.size();i++)
    {
        delete players[i];
    }
    players.clear();
    for(size_t i = 0; i < playersInfo.size();i++)
    {
        delete playersInfo[i];
    }
    playersInfo.clear();
    for(size_t i = 0; i < bots.size();i++)
    {
        delete bots[i];
    }
    bots.clear();
    for(size_t i = 0; i < BotsInfo.size();i++)
    {
        delete BotsInfo[i];
    }
    BotsInfo.clear();
    close(socketServer);
}

void Server::serverInit(int numb)
{
    socketServer = socket(AF_INET,SOCK_STREAM,0);
    if(socketServer < 0)
    {
        perror("Bad socket");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(1488);
    addr.sin_addr.s_addr = INADDR_ANY;

    if((bind(socketServer,(sockaddr*)&addr,sizeof(addr)))<0)
    {
        perror("Bad bind!");
        exit(-1);
    }

    listen(socketServer,10);

    numbOfPlayers = numb;
    numbOfBots = 4 - numb;

    threads.resize(4);
    players.resize(numbOfPlayers);
    playersInfo.resize(numbOfPlayers);
    bots.resize(numbOfBots);
    BotsInfo.resize(numbOfBots);
//    scores.scores.resize(4);

    coordinates.set_grid(grid);

    state = START;
}

void Server::doServer()
{
    std::chrono::seconds threadStop(1);
    std::chrono::seconds waiting(3);
    for(size_t i = 0; i < numbOfPlayers; i++)
    {
        int accepted = accept(socketServer,0,0);
        //Принимаем сокет клиента
        players[i] = new Player(accepted,grid,i,&semafor);
        //Создаём объект игрока в куче
        coordinates.set(i,&players[i]->realX,&players[i]->realY);
        //Запоминаем где брать координаты для отсылки на клиент
        scores.scores[i] = &players[i]->score;
        //Запоминаем, где брать очки
        playersInfo[i] = new PlayerInfo(&state,players[i],&coordinates,&scores);
        //Создаём и заполняем структуру для потоковой функции
        pthread_create(&threads[i],0,playerServis,static_cast<void*>(playersInfo[i]));
        //запускаем поток для игрока
        std::this_thread::sleep_for(threadStop);
        //Ждём секунду, необходимо для генерации случайных координат
    }
    for(size_t i = numbOfPlayers, j = 0; i < 4; i++,j++)
    {
        bots[j] = new Bot(grid,&semafor);
        //Создаём нового бота
        BotsInfo[j] = new BotInfo(bots[j],&state);
        //Создаём и заполняем структуру для потоковой функции
        scores.scores[i] = &bots[j]->score;
        //Запоминаем, где брать очки
        coordinates.set(i,&bots[j]->realX,&bots[j]->realY);
        //Запоминаем где брать координаты для отсылки на клиент
        pthread_create(&threads[i],0,botServis,static_cast<void*>(BotsInfo[j]));
        //запускаем поток для бота
        std::this_thread::sleep_for(threadStop);
        //Ждём секунду, необходимо для генерации случайных координат
    }

    state = WAITING;

    std::this_thread::sleep_for(threadStop);

    state = STARTGAME;

    int summ;
    //Ждём, когда все точки будут съедены
    while (summ < 222)
    {
        summ = 0;
        for(size_t i = 0; i < players.size(); i++)
        {
            summ += players[i]->get_score();
        }
        for(size_t i = 0; i < bots.size(); i++)
        {
            summ += bots[i]->get_score();
        }
        std::this_thread::sleep_for(threadStop);
    }

    state = RESULTS;

    for(size_t i = 0; i < 4; i++)
    {
        pthread_join(threads[i],NULL);
    }
    close(socketServer);

}

void *Server::playerServis(void *arg)
{
    PlayerInfo* info = static_cast<PlayerInfo*>(arg);
    std::chrono::milliseconds dude(33);
    unsigned short flag = 0;
    char directionFromClient;
    unsigned short scoreToClient;
    while(true)
    {
        switch (*info->state)
        {
        case START:
                std::this_thread::sleep_for(dude);
                break;
        case WAITING:
                send(info->player->socketPlayer,&flag,sizeof(unsigned short),0);
                //Флаг о том, что мы ждём начала игры, но необходимо отрисовать поле
                info->coordinates->make_frame();
                //Создаём координаты фрейма
                //send(info->player->socketPlayer,info->coordinates->grid,sizeof(short[400]),0);
                for(size_t i = 0; i < 400; i++)
                {
                    send(info->player->socketPlayer,&info->coordinates->grid[i],sizeof(short),0);
                }
                for(size_t i = 0; i < 4; i++)
                {
                    send(info->player->socketPlayer,&info->coordinates->frameX[i],sizeof(double),0);
                    send(info->player->socketPlayer,&info->coordinates->frameY[i],sizeof(double),0);
                }
                //Отсылаем координаты игроков и само поле
                send(info->player->socketPlayer,&info->player->ID,sizeof(int),0);
                std::this_thread::sleep_for(dude);
                break;
        case STARTGAME:
                info->player->eat();
                flag = 1;
                //Изменяем флаг
                send(info->player->socketPlayer,&flag,sizeof(unsigned short),0);
                //Отправляем флаг, что игра началась
                recv(info->player->socketPlayer,&directionFromClient,sizeof(char),0);
                info->player->set_direction(directionFromClient);
                //Принимаем направление движения
                if(info->player->move())
                {
                    for(size_t i = 0; abs(i - 1/info->player->get_speed()) > 0.001; i++)
                    {
                        info->player->step();
                        info->coordinates->make_frame();
                        //send(info->player->socketPlayer,info->coordinates->grid,sizeof(short[400]),0);
                        for(size_t i = 0; i < 400; i++)
                        {
                            send(info->player->socketPlayer,&info->coordinates->grid[i],sizeof(short),0);
                        }
                        for(size_t i = 0; i < 4; i++)
                        {
                            send(info->player->socketPlayer,&info->coordinates->frameX[i],sizeof(double),0);
                            send(info->player->socketPlayer,&info->coordinates->frameY[i],sizeof(double),0);
                        }
                        std::this_thread::sleep_for(dude);
                    }
                    info->scores->make_frame();
                    for(size_t i = 0; i < 4; i++)
                    {
                        send(info->player->socketPlayer,&info->scores->frameScores[i],sizeof(unsigned short),0);
                    }
                }
                else
                {
                    for(size_t i = 0; abs(i - 1/info->player->get_speed()) > 0.001; i++)
                    {
                        info->coordinates->make_frame();
                        //send(info->player->socketPlayer,info->coordinates,sizeof(Coordinates),0);
                        send(info->player->socketPlayer,info->coordinates->grid,sizeof(short[400]),0);
                        for(size_t i = 0; i < 4; i++)
                        {
                            send(info->player->socketPlayer,&info->coordinates->frameX[i],sizeof(double),0);
                            send(info->player->socketPlayer,&info->coordinates->frameY[i],sizeof(double),0);
                        }
                        std::this_thread::sleep_for(dude);
                    }
                    info->scores->make_frame();
                    for(size_t i = 0; i < 4; i++)
                    {
                        send(info->player->socketPlayer,&info->scores->frameScores[i],sizeof(unsigned short),0);
                    }
                }
                break;
        case RESULTS:
                flag = 2;
                send(info->player->socketPlayer,&flag,sizeof(unsigned short),0);
                info->scores->make_frame();
                for(size_t i = 0; i < 4; i++)
                {
                    send(info->player->socketPlayer,&info->scores->frameScores[i],sizeof(unsigned short),0);
                }
                pthread_exit(0);
                break;
        default:
                std::this_thread::sleep_for(dude);
                break;
        }
    }


}

void *Server::botServis(void *arg)
{
    BotInfo* info = static_cast<BotInfo*>(arg);
    std::chrono::milliseconds dude(33);

    while (true)
    {
        switch (*info->state)
        {
            case START:
                std::this_thread::sleep_for(dude);
                break;
            case WAITING:
                std::this_thread::sleep_for(dude);
                break;
            case STARTGAME:
                info->bot->eat();
                info->bot->move();
                for(size_t i = 0; abs(i - 1/info->bot->get_speed()) > 0.001; i++)
                {
                    info->bot->step();
                    std::this_thread::sleep_for(dude);
                }
                break;
            case RESULTS:
                pthread_exit(0);
            default:
                std::this_thread::sleep_for(dude);
                break;
        }
    }

}
