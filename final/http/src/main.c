#include <stdio.h>
#include <stdlib.h>
#include "http_server.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("./app <port>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    // string to int
    run_http_server(port);

    return 0;
}
