/*
 * =====================================================================================
 *
 *       Filename:  initserver.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月17日 12时52分34秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>

int initserver(int type,const struct sockaddr *addr,socklen_t alen,int qlen)
{

    //printf("addr port %d\n",((sockaddr_in*)addr)->sin_port);

    int fd;
    int err = 0;

    if(fd = socket(addr->sa_family,type,0) < 0){
        return -1;
    }

    if(bind(fd,addr,alen) < 0){
        err = errno;
        goto errout;
    }
    if(type == SOCK_STREAM){
        if(listen(fd,qlen) < 0){
            err = errno;
            goto errout;
        }

    }

    return fd;
errout:
    close(fd);
    errno = err;
    return -1;
}
