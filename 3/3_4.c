#include<stdio.h>    
#include<string.h>                                                                                             
#include<stdlib.h>    
#include<unistd.h>    
#include<sys/wait.h>  

int main()
{

    pid_t pid=fork();
    int stat;
    if(pid<0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if(pid==0)
    {
        printf("this is the child process\n");
       
        exit(EXIT_SUCCESS);
    }else {
        wait(&stat);
       
        printf("id:%d\n",getpid());
        printf("id:%d\n",getppid());
    }
}