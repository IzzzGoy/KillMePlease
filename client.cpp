#include "client.h"

Client::Client()
{
    frameX.resize(4);
    frameY.resize(4);
    for(size_t i = 0; i < 4; i++)
    {
        scores.frameScores[i] = 0;
    }
}

Client::~Client()
{
    close(socketClient);

}

void Client::start_client(char *address)
{
    socketClient = socket(AF_INET,SOCK_STREAM,0);
    if(socketClient < 0)
    {
        perror("Bad socket!");
        exit(-1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(9488);
    addr.sin_addr.s_addr = inet_addr(address);

    connect(socketClient,(sockaddr*)&addr,sizeof(addr));

//    fcntl(socketClient, F_SETFL, O_NONBLOCK);
}

void Client::close_client()
{
    close(socketClient);
}

bool Client::protocol()
{
    std::chrono::milliseconds dude(33);
    unsigned short flag;
    recv(socketClient,&flag,sizeof(unsigned short),0);
    switch (flag)
    {
    case 0:

        recv(socketClient,grid,sizeof(short[400]),0);
        for(size_t i = 0; i < 4; i++)
        {
            recv(socketClient,&frameX[i],sizeof(double),0);
            recv(socketClient,&frameY[i],sizeof(double),0);
        }
        recv(socketClient,&id,sizeof(int),0);
        return true;
        break;
    case 1:
        send(socketClient,&direction,sizeof(char),0);
        for(size_t i = 0; abs(i - 1/0.1) > 0.001; i++)
        {
//            recv(socketClient,&coordinates,sizeof(Coordinates),0);
            recv(socketClient,grid,sizeof(short[400]),0);
            for(size_t i = 0; i < 4; i++)
            {
                recv(socketClient,&frameX[i],sizeof(double),0);
                recv(socketClient,&frameY[i],sizeof(double),0);
            }
        }
        for(size_t i = 0; i < 4; i++)
        {
            recv(socketClient,&scores.frameScores[i],sizeof(unsigned short),0);
        }
        return true;
        break;
    case 2:
        for(size_t i = 0; i < 4; i++)
        {
            recv(socketClient,&scores.frameScores[i],sizeof(unsigned short),0);
        }
        return false;
        break;
    default:
        break;
    }
}

void Client::set_direction(char direction)
{
    this->direction = direction;
}
