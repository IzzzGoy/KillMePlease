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
    addr.sin_port = htons(5488);
    addr.sin_addr.s_addr =inet_addr(address);

    connect(socketClient,(sockaddr*)&addr,sizeof(addr));

}

void Client::close_client()
{
    close(socketClient);
}

bool Client::protocol()
{
    int count = 0;
    std::chrono::milliseconds dude(33);
    unsigned short flag;
    recv(socketClient,&flag,sizeof(unsigned short),0);
    switch (flag)
    {
    case 0:
        while(count != sizeof(short[400]))
        {
            count += recv(socketClient,grid,sizeof(short[400]),0);
        }
        count = 0;
        while(count != sizeof(double)*8)
        {
            for(size_t i = 0; i < 4; i++)
            {
                count += recv(socketClient,&frameX[i],sizeof(double),0);
                count += recv(socketClient,&frameY[i],sizeof(double),0);
            }
        }
        count = 0;
        while(count != sizeof(int))
        {
            count += recv(socketClient,&id,sizeof(int),0);
        }
        count = 0;
        return true;
        break;
    case 1:
        while (count != sizeof(char))
        {
            count += send(socketClient,&direction,sizeof(char),0);
        }
        count = 0;
        for(size_t i = 0; abs(i - 1/0.1) > 0.001; i++)
        {
            while(count != sizeof(short[400]))
            {
                count += recv(socketClient,grid,sizeof(short[400]),0);
            }
            count = 0;
            while(count != sizeof(double)*8)
            {
                for(size_t i = 0; i < 4; i++)
                {
                    count += recv(socketClient,&frameX[i],sizeof(double),0);
                    count += recv(socketClient,&frameY[i],sizeof(double),0);
                }
            }
            count = 0;
        }
        while(count != sizeof(unsigned short)*4)
        {
            for(size_t i = 0; i < 4; i++)
            {
                count += recv(socketClient,&scores.frameScores[i],sizeof(unsigned short),0);
            }
        }
        return true;
        break;
    case 2:
        while(count != sizeof(unsigned short)*4)
        {
            for(size_t i = 0; i < 4; i++)
            {
                count += recv(socketClient,&scores.frameScores[i],sizeof(unsigned short),0);
            }
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
