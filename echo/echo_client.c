#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>
#include "../unp_prac.h"
#include "../lib/inet_pton.h"




/**
 * 这是客户端的代码，步骤为下
 * 1.socket返回socket描述符
 * 2.设置seraddr
 * 3.connect
 * 4.read socket描述符
 */


//负责从标准输入读取一行写到服务器上，并且从服务器读取一行写到标准输出
void echo_client(FILE *fp, int sockfd);

int main(int argc, char **argv) {
    int sockfd, n;
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
        exit(0);
    };

    echo_client(stdin, sockfd);

}

void echo_client(FILE *fp, int sockfd) {
    char sendline[MAXLINE], recvline[MAXLINE];

    // tips:
    // char *fgets(char* buf,int n,FILE* fp);buff为自己设定的缓冲，n为缓冲区大小。
    // 读到\n或者n-1字符结束，第n个字符永远是NULL
    //!!!终止条件（UNP p103）：
    // 输入EOF时，fgets返回空指针。从而返回到main,进程执行完毕终止。
    // 这时内核负责关闭所有打开的描述符，因此套接字由内核关闭。
    // 这导致客户TCP发送一个FIN给服务器，服务器返回一个ACK，这是TCP终止的前半部分。这时服务器处于CLOSE_WAIT,客户端处于TIME_WAIT2
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));
        if (Readline(sockfd, recvline, MAXLINE) == 0)
            err_quit("echo_client client:server terminated permanturely");
        fputs(recvline, stdout);
    }
}