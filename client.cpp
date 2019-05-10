#include "client.h"

Client::Client()
{

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
    addr.sin_port = htons(1488);
    addr.sin_addr.s_addr = inet_addr(address);

    connect(socketClient,(sockaddr*)&addr,sizeof(addr));

//    fcntl(socketClient, F_SETFL, O_NONBLOCK);
}

void Client::close_client()
{
    close(socketClient);
}

void Client::protocol()
{
    std::chrono::milliseconds dude(33);
    unsigned short flag;
    recv(socketClient,&flag,sizeof(unsigned short),0);
    switch (flag)
    {
    case 0:
        recv(socketClient,&coordinates,sizeof(Coordinates),0);
        recv(socketClient,&id,sizeof(int),0);
        break;
    case 1:
        send(socketClient,&direction,sizeof(char),0);
        for(size_t i = 0; abs(i - 1/0.1) > 0.001; i++)
        {
            recv(socketClient,&coordinates,sizeof(Coordinates),0);
        }
        recv(socketClient,&score,sizeof(unsigned short),0);
        break;
    case 2:
        recv(socketClient,&score,sizeof(unsigned short),0);
        recv(socketClient,&scores,sizeof(Scores),0);
        break;
    default:
        break;
    }
}

void Client::set_direction(char direction)
{
    this->direction = direction;
}
