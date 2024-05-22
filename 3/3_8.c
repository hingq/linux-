#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_COMMAND_LENGTH 100


void parest(char*buff)
{
    char *p=buff,*f=NULL;
    while(*p!='\0')
    {
        if(*P=='>')
        {
            p++;
            f=p;
            break;
        }
        p++;
    }
    // 覆盖
    int fd=open(f,O_CREAT|O_TRUNC|O_WRONLY,0644);
    dup2(fd,1);
    
}

void dup_()
{
    
}
int main()
{
    char command[MAX_COMMAND_LENGTH];
    char *program;
    int status;

    fgets(command,MAX_COMMAND_LENGTH,stdin);
    //获取键盘输入
    // printf("use system\n");
    // system(command);

    pid_t pid=fork();
    if(pid<0) {
        perror("fork");
        exit(EXIT_FAILURE);

    } else if(pid==0)
   {
    char *token;
    token=strtok(command,"\n");
    execl("/bin/sh","sh","-c",token,NULL);
    // /bin/sh shell解释器
    // 路径 命令行参数 -c  参数后面的值当作命令行，而不是普通的字符串
    // 把当前进程替换为一个新进程 execl
   } else {
    waitpid(pid,NULL,0);
   }
   return 0;
}