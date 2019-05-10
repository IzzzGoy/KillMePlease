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

    int semId;
public:
    void Give(int Number);
    void Take(int Number);
    ~Semafor();
    Semafor();
};

#endif // SEMAFOR_H
