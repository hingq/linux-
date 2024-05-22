#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/sem.h>
#define MAX_SIZE 1024

int main () {

    key_t key[3];
    for(int i=0;i<3;i++){
         key[i]=ftok(".",126+i);
    }
    int shmid = shmget(key[0], MAX_SIZE, IPC_CREAT | 0666); 
    int semid=semget(key[1],1,0666|IPC_CREAT);
    int msgid=msgget(key[2], 0666 | IPC_CREAT);
    system("ipcs");
    return 0;
}

