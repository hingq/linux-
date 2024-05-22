#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_SIZE 1024

int main() {
    key_t key = ftok(".",1); 
    // 创建共享内存的键值
    
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666); 
    // 创建共享内存

    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    /* 用来启动对该共享内存的访问，并把共享内存连接到当前进程的地址空间
        返回一个指向共享内存第一个字节的指针
    */
     if (shm_ptr == (char *)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }


    FILE *fp = popen("ls -l", "r"); // 执行 ls -l 命令并将结果复制到共享内存
        // popen 创建一个管道，调用fork()产生一个子进程
    fread(shm_ptr, sizeof(char), SHM_SIZE, fp);
    pclose(fp);
    shmdt(shm_ptr); //将共享内存从当前进程中分离 并不是删除
    // shmctl(shmid, IPC_RMID, NULL); // 删除共享内存段
    return 0;
}
