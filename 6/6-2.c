#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>

#define MSG_SIZE 1024
struct msg_st
{
    long int msg_type;
    char text[MSG_SIZE];
};

int main() {
    struct msg_st message;
    key_t key = ftok(".",2); 
    // 创建共享内存的键值
    int msg_id=msgget(key, 0666 | IPC_CREAT);
    // 如果共享内存不存在，则创建一个共享内存，否则打开操作
    if(msg_id==-1){
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    FILE *fp = popen("ls -l", "r"); 
    message.msg_type=1;
    fread(message.text,sizeof(char),MSG_SIZE,fp);
    if (msgsnd(msg_id, (void *)&message, sizeof(message.text), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }
    pclose(fp);
    return 0;
}
  
