#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "unp_prac.h.h"
#include "lib/inet_pton.h"

//
// Created by arloor on 17-9-3.
//
int main(int argc,char **argv){
    int sockfd,n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    char* servIP;

    if(argc!=2){
        servIP="127.0.0.1";
    }else{
        servIP=argv[0];
    }

    /*
     * 调用socket函数
     */
    //socket函数创建一个网ji（AF_INET）字节流（SOCK_STREAM）套接字,这是TCP套接字的花哨名字。
    //返回一个小整数的描述符。
    if( (sockfd=socket(AF_INET,SOCK_STREAM,0))<0){
        printf("调用socket函数失败");
    }

    /*
     * 设置servaddr(服务器的端口和地址)
     */
    //首先清空servaddr结构体,作用等同于memset
    //书上推荐使用bzero是因为只有两个参数，比较好记
    bzero(&servaddr,sizeof servaddr);
    servaddr.sin_family=AF_INET;//置地址族为AF_INET
    servaddr.sin_port=htons(13);//htons函数负责将整数转换成二进制端口号
    if(inet_pton(AF_INET,&servIP,&servaddr.sin_addr)<=0){
        printf("转换IP地址到二进制失败");
    }

    /*
     * 连接
     */
}
