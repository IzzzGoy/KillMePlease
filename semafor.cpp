#include "semafor.h"

Semafor::Semafor()
{
    semId = semget(IPC_PRIVATE, 400, 0600|IPC_CREAT);

}
void Semafor::Get(int Number)
{
    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = Number;
}

void Semafor::Take(int Number)
{
        op.sem_op = -1;
        op.sem_flg = 0;
        op.sem_num = Number;
}

Semafor::~Semafor()
{
    semctl(semId, IPC_RMID, 0);
}

void Semafor::Stop()
{
    semop(semId, &op, 0);
}
