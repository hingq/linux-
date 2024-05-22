#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int my_open(char *str,char *mode)
{
    int flag;
    switch(mode[0]){
        case 'r':
            flag=O_RDONLY;
            break;
        case 'w':
            flag=O_WRONLY | O_CREAT | O_TRUNC;
        case 'a':
            flag=O_WRONLY | O_CREAT | O_APPEND;
        default:
            flag=-1;
    }
    int fd=open(str,flag,0644);
    if(fd=-1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    return fd;
}
void  my_close(int fd){
    close(fd);
    return ;
}
int main()
{
    int fd=my_open("password.txt","w");

    char str[20]="1213131"
    if(write(fd,&str,strlen(str))==-1){
        perror("write");
        exit(EXIT_FAILURE);
    }
    my_close(fd);
    return 0;
}