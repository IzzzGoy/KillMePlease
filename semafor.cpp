#include "semafor.h"

Semafor::Semafor()
{
    semId = semget(IPC_PRIVATE, 400, 0600|IPC_CREAT);
        struct sembuf op;
        for(size_t i = 0; i < 400; i++)
        {
            op.sem_op = 1;
            op.sem_flg = 0;
            op.sem_num = i;

            semop(semId, &op, 0);
        }

}
void Semafor::Give(int Number)
{
    struct sembuf op;

    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = Number;

    semop(semId, &op, 1);
}

void Semafor::Take(int Number)
{
        struct sembuf op;

        op.sem_op = -1;
        op.sem_flg = 0;
        op.sem_num = Number;

        semop(semId, &op, 1);
}

Semafor::~Semafor()
{
    semctl(semId, IPC_RMID, 0);
}
