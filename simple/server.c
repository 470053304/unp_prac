#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>
#include "../unp_prac.h"


//编译：
//gcc server.c -o server
//运行：
//./server

/**
 * 这是server端的程序，流程总结为：
 * 1.函数socket返回listenfd
 * 2.设置servaddr(先使用bzero);
 * 3.bind，绑定seraddr和listenfd
 * 4.listen，将套接字转换成监听套接字。
 * ---以上socket bind listen 是任何TCP服务器准备监听描述符的正常步骤
 * 5.无穷循环for(;;)
 * 6.调用accept（等待某个连接到达并被内核接受），
 *      在accept中，TCP连接使用所谓的三路握手建立连接，完毕则返回已连接描述符（confd），用于与客户进程通信。
 * 7.关闭confd，终止连接。
 * @param argc
 * @param argv
 * @return
 */

int main(int argc, char **argv) {
    int listenfd, confd;
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    time_t ticks;


    //socket函数创建一个网际（AF_INET）字节流（SOCK_STREAM）套接字,这是TCP套接字的花哨名字。
    //返回一个小整数的描述符。
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("调用socket函数失败");
    }

    //设置服务器
    //将servaddr内存区域清0;
    bzero(&servaddr, sizeof servaddr);

    //设置seraddr
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//设为any：如果服务器有多个网络接口，那么服务器进程可以在所有接口上接受用户连接
    servaddr.sin_port = htons(port);/*时间服务的端口为13*/

    //bind
    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0) {
        printf("\033[31mbind失败,端口未被系统释放\n请修改unp_prac.h中的port，然后clean，重新编译运行\n\033[0m");
        exit(0);
    };

    //listen
    listen(listenfd, LISTENQ);
    printf("创建监听连接符，等待连接\n");
    while (1) {

        confd = accept(listenfd, (SA *) NULL, NULL);
        printf("接受连接，返回时间\n");
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "\033[35m%.24s\n\033[0m", ctime(&ticks));
        write(confd, buff, strlen(buff));

        close(confd);
    }


    return 0;
}