#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 1024


int main() {

    key_t key=ftok(".",1);
    // 建立通讯需要的键值
    int shmid=shmget(key,SHM_SIZE,0666);
    if(shmid==-1){
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    char *shm_ptr=(char*) shmat(shmid,NULL,0);
    // 指向共享内存第一个字节的指针
    if(shm_ptr==(char*)-1){
        perror("shmat");
        exit(EXIT_FAILURE);
    } 

    printf("received data:%s",shm_ptr);
    shmdt(shm_ptr);
    return 0;
}