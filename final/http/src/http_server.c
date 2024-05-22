#include "network.h"
#define MAX_SIZE 1024
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
#include <string.h>
// 发生函数
int HTTP_ServerSendFile(int client_fd, char *buff, char *type, char *file)
{

    int fd = open(file, 2);
    if (fd < 0)
        return -1;
    // 获取文件状态信息
    struct stat s_buff;
    fstat(fd, &s_buff);

    // 响应头信息
    sprintf(buff, "HTTP/1.1 200 OK\r\nContent-type:%s\r\nContent-Length:%ld\r\n\r\n", type, s_buff.st_size);

    // 强制转换为ssize_t
    if (write(client_fd, buff, strlen(buff)) != (ssize_t)strlen(buff))
        return -2;

    ssize_t cnt;
    while (1)
    {
        cnt = read(fd, buff, 1024);
        if (write(client_fd, buff, cnt) != cnt)
            return -3;
        if (cnt != 1024)
            break;
    }
    return 0;
}

/*线程工作函数*/
void *thread_func(void *argv)
{
    int client_fd = *(int *)argv;
    free(argv);
    //  防止内存泄漏
    ssize_t cnt;
    char buff[1024];
    // 读取浏览器发送过来的数据
    cnt = read(client_fd, buff, 1024);
    if (cnt < 0)
    {
        perror("read failed");
        close(client_fd);
        pthread_exit(NULL);
    }
    // 打印接收到的字节数
    printf("Received %zd bytes from the client.\n", cnt);

    // 添加 null 终止符
    buff[cnt] = '\0';

    // 打印接收到的消息
    printf("Receive message from客户端:%s\n", buff);
    if (strstr((const char *)buff, "GET / HTTP/1.1"))
    {
        HTTP_ServerSendFile(client_fd, buff, "text/html", "./test/test.html");
    }
    else if (strstr((const char *)buff, "GET /1.jpg HTTP/1.1"))
    {
        HTTP_ServerSendFile(client_fd, buff, "image/jpeg", "./1.jpg");
    }
    else
    {

        int fd = open("./file/recv.md", O_WRONLY | O_CREAT | O_APPEND, 0666);
        if (fd < 0)
        {
            perror("open");
        }
        if (write(fd, buff, strlen(buff)) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    char buffer[MAX_SIZE];
    ssize_t num_bytes;
    while ((num_bytes = recv(client_fd, buffer, MAX_SIZE - 1, 0)) > 0)
    {
        buffer[num_bytes] = '\0';
        printf("%s", buffer);
    }
    if (num_bytes == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    close(client_fd);
    // 退出线程
    pthread_exit(NULL);
}

// run
void run_http_server(int port)
{
    pthread_t thread_id;
    int server_fd;
    if (port < 10000)
    {
        server_fd = create_server_socket(port);
    }
    else
    {
        server_fd = creatIpv6(port);
    }

    if (server_fd < 0)
    {
        printf("Failed to create server socket.\n");
        exit(1);
    }

    while (1)
    {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept_connection(server_fd);
        if (*client_fd < 0)
        {
            printf("Failed to accept connection.\n");
            continue;
        }
        // 线程
        if (pthread_create(&thread_id, NULL, thread_func, client_fd) != 0)
        {
            printf("线程创建失败.\n");
            break;
        }
        // 分离线程，避免资源泄漏
        pthread_detach(thread_id);
    }
}
