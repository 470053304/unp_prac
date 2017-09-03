//
// Created by arloor on 17-9-3.
//

#ifndef UNP_PRAC_UNP_PRAC_H_H
#define UNP_PRAC_UNP_PRAC_H_H

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */



#endif //UNP_PRAC_UNP_PRAC_H_H
