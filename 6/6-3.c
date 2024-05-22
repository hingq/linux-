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
	int fd, i;
	time_t now;
	long randata;
	char *message;
	char temp[64] = {0};
    key_t key =ftok(".",3);
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
		printf("error usage!\nusage: server filename\n");
		exit(EXIT_FAILURE);
	}

    wait_semaphore(semid);

	/*create a file for communicate */
	if ((fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644)) == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}

	/*server-side linked file */
	while (1) {
		srandom(time(&now));
        // 产生随机数
		randata = random();
		i = 62;
		temp[63] = '\0';

		while (randata > 0) {
			temp[i--] = randata % 10 + 48;
			randata /= 10;
		}
		message = temp + i + 1;

        //返回文件开头位置 
		if ((lseek(fd, 0, SEEK_SET)) == -1) {
			perror("lseek");
			exit(EXIT_FAILURE);
		}
        // 写数据
		if (write(fd, message, strlen(message)) == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}
		sleep(1);
	}
}
