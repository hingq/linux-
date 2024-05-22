#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_SIZE 1024
#define MSG_TYPE 1

struct msg_st
{
    long int msg_type;
    char text[MSG_SIZE];
};

int main() {
    key_t key = ftok(".", 2); // 创建消息队列的键值
    int msgid = msgget(key, 0666 | IPC_CREAT); // 获取消息队列
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    struct msg_st message;
    // 从消息队列中接收消息
    if (msgrcv(msgid, (void*)&message, MSG_SIZE, MSG_TYPE, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }
    printf("Received result :\n%s\n", message.text); // 打印接收到的消息

    return 0;
}
