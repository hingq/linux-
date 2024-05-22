/*
 *File: daemon_example.c
 *This is a daemon example.
 *Written by Yang Shu
 *ALL rights reserved.
 *Data: 2023-10-12
 *Version:1.0
 */


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<fcntl.h>
#include<sys/syslog.h>
#include<sys/param.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>

void init_daemon(const char * cmd, int para)
{
    int pid;
	int i;
	signal(SIGHUP,SIG_IGN); //忽略sighup
	umask(0); //创建文件使用系统默认权限

	if((pid=fork())>0) //创建进程
	   exit(EXIT_SUCCESS);
	else if(pid<0)
	   {
	      perror("fork1");
	      exit(EXIT_FAILURE);
	   }
	setsid(); //创建新的会话

	if((pid=fork())>0)
	    exit(EXIT_SUCCESS);
        else if(pid<0){
	    perror("fork2");
	    exit(EXIT_FAILURE);
	   }
    chdir("/"); //切换到 /
    for(i=0; i<NOFILE; i++) //关闭文件描述符
	   close(i);
	 int fd=open("/home/daemon.sys",O_WRONLY|O_CREAT|O_APPEND,0644);
     if(fd<0){
        perror("open");
        exit(EXIT_FAILURE);
     }
     char buffer[100];
     time_t now=time(NULL); //时间戳
     snprintf(buffer,sizeof(buffer),"pid %d,time %s",getpid(),ctime(&now));
     //拼接pid time
     if(write(fd,buffer,strlen(buffer))<0)
     {
        perror("write");
        close(fd);
        exit(EXIT_FAILURE);
     }
    close(fd);
	return;
}

int main(int argc, char * argv[])
{
    time_t ticks;
	init_daemon(argv[0], LOG_KERN);
	
	while(1){
	  sleep(3);
	  ticks = time(NULL);
	  syslog(LOG_INFO,"%s",asctime(localtime(&ticks)));
      //写入系统文件
	}
}
