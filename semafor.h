#ifndef SEMAFOR_H
#define SEMAFOR_H

#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>

class Semafor
{
private:
    struct sembuf op;
    int semId;
public:
    void Get(int Number);
    void Take(int Number);
    void Stop();
    ~Semafor();
    Semafor();
};

#endif // SEMAFOR_H
