//
// Created by arloor on 17-11-17.
//

#ifndef UNP_PRAC_SELECT_SERVER_H
#define UNP_PRAC_SELECT_SERVER_H


#include "../unp_prac.h"
#include <list>

using namespace std;

typedef list<int>::const_iterator LI;

class Select_Server {
private:
    int listenfd;
    SA cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    struct sockaddr_in servaddr;

    list<int> readfds;
    list<int> writefds;
    fd_set readset;
    fd_set writeset;
    int maxfdp1 = 0;

    void updateMaxfdp1(int newfd);

    void initfdset();

    int Select();

    list<int> getfds();

    void echo_server(int sockfd);

    void delelefd(int fd);

public:
    Select_Server();

    ~Select_Server();

    void service();

    void addReadfd(int readfd);

    void addWrite(int writefd);
};


#endif //UNP_PRAC_SELECT_SERVER_H
