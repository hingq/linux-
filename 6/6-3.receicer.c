#include <stddef.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<fcntl.h>
#include<time.h>
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
int main(int argc, char *argv[])
{
	int fd, len;
	char buf[128];
    key_t key=ftok(".",3);
    int semid=semget(key,1,0666|IPC_CREAT);
    if(semid==-1){
        perror("semget");
        exit(EXIT_FAILURE);
    }

     union semun arg;
    arg.val = 1;

    if (semctl(semid, 0, SETVAL, arg) == -1) {
        // setval 把信号量初始化为一个已知的值
        perror("semctl");
        exit(EXIT_FAILURE);
    }

	if (argc != 2) {
		printf("error usage!\nusage: client filename\n");
		exit(EXIT_FAILURE);
	}

    wait_semaphore(semid);

	/*create file*/
	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	/*client-side linked file */
	while ((len = read(fd, buf, 128)) > 0) {
		write(1, buf, len - 1);
        // 1 是标准输出的描述符
	}
    printf("\n");
	close(fd);
}
