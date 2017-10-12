#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <zconf.h>
#include "../unp_prac.h"
#include "../lib/inet_pton.h"




/**
 * 这是客户端的代码，步骤为下
 * 1.socket返回socket描述符
 * 2.设置seraddr
 * 3.connect
 * 4.read socket描述符
 */

/**
 * 编译
 * gcc -o client client.c ../lib_io/readn.c ../lib/error.c
 */

int main(int argc, char **argv) {
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    char *servIP;

    if (argc != 2) {
        servIP = "127.0.0.1";
    } else {
        servIP = argv[0];
    }

    //socket函数创建一个网ji（AF_INET）字节流（SOCK_STREAM）套接字,这是TCP套接字的花哨名字。
    //返回一个小整数的描述符。
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("调用socket函数失败");
    }


    //首先清空servaddr结构体,作用等同于memset
    //书上推荐使用bzero是因为只有两个参数，比较好记
    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;//置地址族为AF_INET
    servaddr.sin_port = htons(port);//htons函数负责将整数转换成二进制端口号
    if ((inet_pton(AF_INET, servIP, &servaddr.sin_addr)) < 0) {
        printf("转换失败");
    }//将ASCII命令行参数转化为正确的形式

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
        printf("连接失败\n");
    };

    while ((n = Readn(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF) {
            printf("写到标准输出失败\n");
        }
    }
    if (n < 0) {
        printf("读取失败\n");
    }

}
