# socket API和TCP/IP协议学习#

-------
- 本博客用于记录socket API和TCP/IP协议的学习
- 目标在2017.1.1之前看完unix网络编程卷一和TCP/IP详解卷一   
- 本博客的环境是opensuse Tumbleweed，其他linux发行版可能会有差异

## 首先放上简单的server/client代码
这两段代码展示了socket连接的一般步骤  

- 注意:这个代码缺少了相关的头文件，不能直接跑哦！  
- 博客对应完整代码见[我的github](https://github.com/arloor/unp_prac)

server.c

```
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <zconf.h>
#include <stdlib.h>
#include "../unp_prac.h"
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
 */

/**
 * 编译：
 * gcc -o server server.c ../lib_io/writen.c ../lib/error.c
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
        Writen(confd, buff, strlen(buff));

        close(confd);
    }
    return 0;
}
```

client.c

```
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
```

##相关文章
[1.套接字编程基础](http://blog.csdn.net/arloor/article/details/78116399)
[2.基本套接字函数](http://blog.csdn.net/arloor/article/details/78121265)