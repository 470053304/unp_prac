#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include "unp_prac.h.h"

int main(int argc,char **argv) {
    int listenfd,confd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE];
    time_t ticks;


    //socket函数创建一个网ji（AF_INET）字节流（SOCK_STREAM）套接字,这是TCP套接字的花哨名字。
    //返回一个小整数的描述符。
    if( (listenfd=socket(AF_INET,SOCK_STREAM,0))<0){
        printf("调用socket函数失败");
    }

    //设置服务器
    //将servaddr内存区域清0;
    bzero(&servaddr,sizeof servaddr);


    printf("Hello, World!\n");
    return 0;
}