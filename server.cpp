#include "server.h"

Server::Server()
{

}

Server::~Server()
{
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
        playersInfo[i] = new PlayerInfo(&state,players[i],&coordinates);
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
        coordinates.set(i,&bots[j]->realX,&bots[j]->realY);
        //Запоминаем где брать координаты для отсылки на клиент
        pthread_create(&threads[i],0,botServis,static_cast<void*>(BotsInfo[j]));
        //запускаем поток для бота
        std::this_thread::sleep_for(threadStop);
        //Ждём секунду, необходимо для генерации случайных координат
    }

    state = WAITING;

    std::this_thread::sleep_for(waiting);

    state = STARTGAME;

    int summ;
    //Ждём, когда все точки будут съедены
    while (summ < 80)
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
    //fcntl(info->player->socketPlayer, F_SETFL, O_NONBLOCK);
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
                send(info->player->socketPlayer,info->coordinates,sizeof(coordinates),0);
                //Отсылаем координаты игроков и само поле
                std::this_thread::sleep_for(dude);
                break;
        case STARTGAME:
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
                        send(info->player->socketPlayer,info->coordinates,sizeof(Coordinates),0);
                        std::this_thread::sleep_for(dude);
                    }
                    info->player->eat();
                    scoreToClient = info->player->get_score();
                    send(info->player->socketPlayer,&scoreToClient,sizeof(unsigned short),0);
                    std::this_thread::sleep_for(dude);
                }
                else
                {
                    for(size_t i = 0; abs(i - 1/info->player->get_speed()) > 0.001; i++)
                    {
                        send(info->player->socketPlayer,info->coordinates,sizeof(Coordinates),0);
                        std::this_thread::sleep_for(dude);
                    }
                    scoreToClient = info->player->get_score();
                    send(info->player->socketPlayer,&scoreToClient,sizeof(unsigned short),0);
                }
                //std::this_thread::sleep_for(dude);
                break;
        case RESULTS:
                flag = 2;
                send(info->player->socketPlayer,&flag,sizeof(unsigned short),0);
                scoreToClient = info->player->get_score();
                send(info->player->socketPlayer,&scoreToClient,sizeof(unsigned short),0);
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
                info->bot->move();
                for(size_t i = 0; abs(i - 1/info->bot->get_speed()) > 0.001; i++)
                {
                    info->bot->step();
                    std::this_thread::sleep_for(dude);
                }
                info->bot->eat();
                std::this_thread::sleep_for(dude);
                break;
            case RESULTS:
                pthread_exit(0);
            default:
                std::this_thread::sleep_for(dude);
                break;
        }
    }

}
