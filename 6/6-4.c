#include <stddef.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
void wait_semaphore(int semid) {

    // 对信号量做减1操作，即等待P（sv）
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = -1; // P=>-1 v=>1
    op.sem_flg = 0;
    semop(semid, &op, 1);
    // 改变信号量的值
}
void send_semaphore(int semid) {
    struct sembuf op;
    op.sem_num = 0;
    op.sem_op = 1; // P=>-1 v=>1
    op.sem_flg = 0;
    semop(semid, &op, 1);
}