#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<stdlib.h>
int main() {
    int fd;
    pid_t pid;
    char buffer[100];
    int nbytes;

    int ret=mkfifo("/tmp/fifo",0666);

    if(ret!=0){
        perror("mkfifo");
        return -1;
    }
    
    pid=fork();

    if(pid==0){
        fd=open("/tmp/fifo",O_RDONLY);//只读
        if(fd<0){
         perror("open fifo");
        }
        char recv[100]={0};

        read(fd,recv,sizeof(buffer));
        printf("received from parent process:\n%s\n",recv);
        close(fd);
    }
    else {
        fd=open("/tmp/fifo",O_WRONLY); //只写
        if(fd<0){
            perror("open fifo ");
            exit(EXIT_FAILURE);
        }
            char send[100]="a message from parent";
            write(fd,send,strlen(send)+1);
            close(fd);
    }
    return 0;

}