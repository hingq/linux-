#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


/*
函数功能: 服务器向客户端发送响应数据
*/
int HTTP_ServerSendFile(int client_fd,char *buff,char *type,char *file)
{
    /*1. 打开文件*/
    int fd=open(file,2);
    if(fd<0)return -1;
    /*2. 获取文件大小*/
    struct stat s_buff;
    fstat(fd,&s_buff);
    /*3. 构建响应头部*/
    sprintf(buff,"HTTP/1.1 200 OK\r\nContent-type:%s\r\nContent-Length:%ld\r\n\r\n",type,s_buff.st_size);
    /*4. 发送响应头*/
    if(write(client_fd,buff,strlen(buff))!=strlen(buff))return -2;
    /*5. 发送消息正文*/
    int cnt;
    while(1)
    {
        cnt=read(fd,buff,1024);
        if(write(client_fd,buff,cnt)!=cnt)return -3;
        if(cnt!=1024)break;
    }
    return 0;
}

/*线程工作函数*/
void *thread_work_func(void *argv)
{
    int client_fd=*(int*)argv;
    free(argv);

    unsigned int cnt;
    unsigned char buff[1024];
    //读取浏览器发送过来的数据
    cnt=read(client_fd,buff,1024);
    buff[cnt]='\0';
    printf("%s\n",buff);

    if(strstr(buff,"GET / HTTP/1.1"))
    {
        HTTP_ServerSendFile(client_fd,buff,"text/html","test.html");
    }
    else if(strstr(buff,"GET /1.jpg HTTP/1.1"))
    {
        HTTP_ServerSendFile(client_fd,buff,"image/jpeg","./1.jpg");
    }
    
    close(client_fd);
    //退出线程
    pthread_exit(NULL);
}

int main(int argc,char **argv)
{   
    if(argc!=2)
    {
        printf("./app <端口号>\n");
        return 0;
    }

    signal(SIGPIPE,SIG_IGN); //忽略 SIGPIPE 信号--防止服务器异常退出

    int sockfd;
    /*1. 创建socket套接字*/
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    /*2. 绑定端口号与IP地址*/
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(atoi(argv[1])); // 端口号0~65535
    addr.sin_addr.s_addr=INADDR_ANY;    //inet_addr("0.0.0.0"); //IP地址
    if(bind(sockfd,(const struct sockaddr *)&addr,sizeof(struct sockaddr))!=0)
    {
        printf("服务器:端口号绑定失败.\n");
    }
    /*3. 设置监听的数量,表示服务器同一时间最大能够处理的连接数量*/
    listen(sockfd,20);

    /*4. 等待客户端连接*/
    int *client_fd;
    struct sockaddr_in client_addr;
    socklen_t addrlen;
    pthread_t thread_id;
    while(1)
    {
        addrlen=sizeof(struct sockaddr_in);
        client_fd=malloc(sizeof(int));
        *client_fd=accept(sockfd,(struct sockaddr *)&client_addr,&addrlen);
        if(*client_fd<0)
        {
            printf("客户端连接失败.\n");
            return 0;
        }
        printf("连接的客户端IP地址:%s\n",inet_ntoa(client_addr.sin_addr));
        printf("连接的客户端端口号:%d\n",ntohs(client_addr.sin_port));

        /*创建线程*/
        if(pthread_create(&thread_id,NULL,thread_work_func,client_fd))
        {
            printf("线程创建失败.\n");
            break;
        }
        /*设置线程的分离属性*/
        pthread_detach(thread_id);
    } 
    /*5. 关闭连接*/
    close(sockfd);
    return 0;
}