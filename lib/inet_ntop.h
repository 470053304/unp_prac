//
// Created by arloor on 17-9-27.
//

#ifndef UNP_PRAC_INET_NTOP_H
#define UNP_PRAC_INET_NTOP_H

#include <sys/socket.h>

//将网络序中的二进制数转化为点分十进制数（p——表达式）
const char *
inet_ntop(int af, const void *src, char *dst, socklen_t size);
/**
 * 定义size是为了防止超出缓冲区
 * size 使用 <netinet/in.h>中定义的
   #define INET_ADDRSTRLEN 16
   #define INET6_ADDRSTRLEN 46
 */

#endif //UNP_PRAC_INET_NTOP_H
