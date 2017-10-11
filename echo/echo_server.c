#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <zconf.h>
#include <stdlib.h>
#include <errno.h>
#include <wait.h>
#include "../unp_prac.h"

//
// Created by arloor on 17-10-3.
//
void sig_chld(int signo);

int main() {
    int connfd, listenfd;
    SA cliaddr;

    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    //bind
    if ((bind(listenfd, (SA *) &servaddr, sizeof(servaddr))) < 0) {
        printf("\033[31mbind失败,端口未被系统释放\n请修改unp_prac.h中的port，然后clean，重新编译运行\n\033[0m");
        exit(0);
    };

    listen(listenfd, LISTENQ);

    signal(SIGCHLD, sig_chld);//定义子进程结束信号的信号处理函数

    socklen_t clilen = sizeof(cliaddr);

    for (;;) {
        int pid;
        connfd = accept(listenfd, &cliaddr, &clilen);
        //下面的if是考虑这样的情形：
        //unp：当阻塞于某个慢系统调用的进程捕获某个信号且相应信号处理函数返回时，系统调用可能会返回一个EINTR错误。
        //有些系统会自动重启某些被中断的系统调用，但是为了可移植性，我们需要对慢系统调用返回EINTR错误做好准备。
        //accept属于这一类慢系统调用。
        if (errno == EINTR) {
            continue;
        }
        if ((pid = fork()) == 0) {
            close(listenfd);
            printf("子进程处理");
            close(connfd);
            exit(0);
        }
        close(connfd);
    }
}

//为了避免僵死进程，每一个子进程exit之后，都会产生SIGCHLD信号
//我们需要设置信号处理函数，wait(pid)来获取子进程终止状态，从而防止子进程成为僵死进程。
void sig_chld(int signo) {
    int statloc;
    pid_t pid;
    /**
     * 下面的while循环中
     * 使用了waitypid函数，第一个参数-1表示任意wait子进程，与wait相同
     * 第二个参数用于保存进程终止状态
     * 第三个参数是option，WNOHANG代表如果没有终止的子进程，不阻塞而是直接返回。
     * 说一下使用while+WNOHANG的原因：
     * unix中信号是不排队的，也就是说，如果在SIGCHLD信号处理的过程中，有其他子进程结束，产生SIGCHLD信号。
     * 新的SIGCHLD信号是不递交的（也就是说，即使有多个子进程结束，即产生多个SIGCHLD，也只会处理一次）。
     * 所以，我们需要在一次信号处理中，肯能需要处理多个终止子程序，因此使用while
     * 至于WNOHANG选项，如果不加上，就阻塞在信号处理程序了。
     */
    while ((pid = waitpid(-1, &statloc, WNOHANG)) > 0) {
        //todo
        //信号处理函数不应该使用printf这类不可重入函数
        printf("子进程%d结束", pid);
    }
    return;
}
