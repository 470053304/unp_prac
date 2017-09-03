//
// Created by arloor on 17-9-3.
//

#ifndef UNP_PRAC_INET_PTON_H
#define UNP_PRAC_INET_PTON_H


//用于转换ip地址（类似127.0.0.1的“表述”转换为二进制的“数”）;
int
inet_pton(int af,const char *src,void *dst);

#endif //UNP_PRAC_INET_PTON_H
