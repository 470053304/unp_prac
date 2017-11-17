//
// Created by arloor on 17-11-17.
//

#include <strings.h>
#include <stdlib.h>
#include <iostream>
#include <zconf.h>
#include "select_echo_server.h"


Select_Server::Select_Server() {
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

    //增加监听套接字
    addReadfd(listenfd);
    initfdset();
}

void Select_Server::updateMaxfdp1(int newfd) {
    if (newfd > maxfdp1 - 1) {
        maxfdp1 = newfd + 1;
    }
}

void Select_Server::addReadfd(int readfd) {
    readfds.push_back(readfd);
    updateMaxfdp1(readfd);
}

void Select_Server::addWrite(int writefd) {
    writefds.push_back(writefd);
    updateMaxfdp1(writefd);
}

Select_Server::~Select_Server() {

}

void Select_Server::service() {
    for (;;) {
        Select();
        list<int> fds = getfds();
        for (LI li = fds.begin(); li != fds.end(); li++) {
            int fd = *li;
            if (fd == listenfd) {
                int connfd = accept(listenfd, &cliaddr, &clilen);
                cout << "new connnection, fd is " << connfd << endl;
                addReadfd(connfd);
            } else {
                cout << "处理描述符：" << fd << endl;
                echo_server(fd);
            };
        }
        initfdset();
    }
}

void Select_Server::initfdset() {
    FD_ZERO(&readset);
    FD_ZERO(&writeset);
    for (LI readli = readfds.begin(); readli != readfds.end(); readli++) {
        const int readfd = *readli;
        FD_SET(readfd, &readset);
    }
    for (LI writeli = writefds.begin(); writeli != writefds.end(); writeli++) {
        const int writefd = *writeli;
        FD_SET(writefd, &writeset);
    }
}

int Select_Server::Select() {
    return select(maxfdp1, &readset, &writeset, nullptr, nullptr);
}

list<int> Select_Server::getfds() {
    list<int> fds;

    for (LI li = readfds.begin(); li != readfds.end(); li++) {
        const int fd = *li;
        if (FD_ISSET(fd, &readset)) {
            fds.push_back(fd);
        }
    }

    for (LI li = writefds.begin(); li != writefds.end(); li++) {
        const int fd = *li;
        if (FD_ISSET(fd, &writeset)) {
            fds.push_back(fd);
        }
    }

    return fds;
}

void Select_Server::echo_server(int sockfd) {
    ssize_t n;
    char buf[MAXLINE];
    again:
    if ((n = read(sockfd, buf, MAXLINE)) > 0) {
        cout << "读到" << n << "个字符，已echo到客户端" << endl;
        write(sockfd, buf, n);
    }
    if (n < 0 && errno == EINTR)//重启因中断而关闭的慢系统调用read
        goto again;
    else if (n < 0)
        cout << "echo_server:读错误" << endl;

    //!!!终止条件 unp 103
    //客户端发送一个FIN给服务器，此时服务器子进程阻塞于read，read返回0(书上就是这么说的，原理不清楚)。
    if (n == 0) {//
        printf("收到FIN分节,客户端正在终止连接\n");
        delelefd(sockfd);
    }
}

void Select_Server::delelefd(int fd) {
    readfds.remove(fd);
    writefds.remove(fd);
}


int main() {
    Select_Server server;
    server.service();
}