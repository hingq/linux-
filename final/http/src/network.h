#ifndef NETWORKING_H
#define NETWORKING_H

int create_server_socket(int port);
int accept_connection(int server_fd);
int creatIpv6(int port);
#endif /* NETWORKING_H */
