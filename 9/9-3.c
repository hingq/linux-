#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in6 server_addr, client_addr;
    socklen_t client_addr_len;
    char buffer[BUFFER_SIZE];
    const char *username = "admin";
    const char *welcome_msg = "Welcome to the server!";

    // 创建 socket
    if ((server_fd = socket(AF_INET6, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址和端口
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;
    server_addr.sin6_port = htons(PORT);

    // 绑定地址和端口
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听连接请求
    if (listen(server_fd, 1) < 0) { // 只允许一个客户端连接
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // 接受客户端连接
        client_addr_len = sizeof(client_addr);
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // 显示客户端的 IP 地址和端口
        char client_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &client_addr.sin6_addr, client_ip, INET6_ADDRSTRLEN);
        printf("New connection from %s:%d\n", client_ip, ntohs(client_addr.sin6_port));

        // 接收客户端发送的用户名
        if (recv(client_fd, buffer, BUFFER_SIZE, 0) < 0) {
            perror("recv failed");
            close(client_fd);
            continue;
        }

        // 比较用户名是否正确
        if (strcmp(buffer, username) == 0) {
            // 发送连接正确的消息给客户端
            if (send(client_fd, welcome_msg, strlen(welcome_msg), 0) < 0) {
                perror("send failed");
            }
        } else {
            // 发送连接错误的消息给客户端
            const char *error_msg = "Invalid";
            if (send(client_fd, error_msg, strlen(error_msg), 0) < 0) {
                perror("send failed");
            }
        }

        // 关闭客户端连接
        close(client_fd);
    }

    return 0;
}
