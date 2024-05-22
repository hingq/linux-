    //用strerror函数实现perror函数的功能
    #include<stdio.h>
    #include<stdlib.h>
    #include<errno.h>
    #include<string.h>
    void  myperror(char * str){
         int errno;
         char *error=strerror(errno);
         printf("%s: %s\n",str,error);
    }
    int main(){
        FILE *fp;
        fp=fopen("~/aaa.txt","r+"); //读写
        if(NULL==fp){
            myperror("openError");
        }
        return 0;
    }
/**
strerror 返回一个指向错误消息字符串的指针。strerror 生成的错误字符串取决于开发平台和编译器。
perror  将一个描述性错误消息输出到标准错误 stderr
*/