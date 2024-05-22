#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
// 创建socket套接字

int creatIpv6(int port)
{
    int server_fd6 = socket(AF_INET6, SOCK_STREAM, 0);

    // 设置 IPv6 服务器地址和端口
    if (server_fd6 < 0)
    {
        perror("socket");
        return -1;
    }
    // 设置 IPv6 地址重用
    if (setsockopt(server_fd6, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt (IPv6) failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in6 server_addr_ipv6;
    memset(&server_addr_ipv6, 0, sizeof(server_addr_ipv6));
    server_addr_ipv6.sin6_family = AF_INET6;
    server_addr_ipv6.sin6_addr = in6addr_any;
    server_addr_ipv6.sin6_port = htons(port);

    if (bind(server_fd6, (struct sockaddr *)&server_addr_ipv6, sizeof(server_addr_ipv6)) < 0)
    {
        perror("bind");
        close(server_fd6);
        return -1;
    }
    if (listen(server_fd6, 5) < 0)
    {
        perror("listen");
        close(server_fd6);
        return -1;
    }
    return server_fd6;
}
int create_server_socket(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        return -1;
    }

    // 设置ipv4地址重用
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt (IPv4) failed");
        exit(EXIT_FAILURE);
    }

    // ipve4
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(server_fd);
        return -1;
    }

    // 最多监听5个请求
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

// 等待套接字连接
int accept_connection(int server_fd)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0)
    {
        perror("accept");
        return -1;
    }
    // 显示客户端的IP地址和端口
    printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    return client_fd;
}
