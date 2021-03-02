/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012-02-17 11:36:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "httprotocol.h"
#include "fileutility.h"
#include "interceptor.h"
#include "yclog/log.h"

#define QLEN 10
#define VISIT_FILE_NAME 100
#define VISIT_FILE_ARGV 100
#define RECEIVE_BUFFER_SIZE 1024
#define SEND_BUFFER_SIZE 1024*10

#define EXIT_NORMAL 0
#define EXIT_ERROR  1

extern int initserver(int,struct sockaddr *,socklen_t,int);

void get_requested_file_info(const char * recv_buffer,uufile_t *fileinfo)
{
    char    visit_file_name_local[VISIT_FILE_NAME]={0,};
    char          visit_file_argv[VISIT_FILE_ARGV]={0,};
    get_visit_file_name_and_argv(recv_buffer,visit_file_name_local,visit_file_argv,VISIT_FILE_ARGV); 

    if( 0 != strlen(visit_file_argv)){
        strncpy(fileinfo->argv,visit_file_argv,ARGV_STRING_MAX);
    }

    strncpy(fileinfo->name,"www/",VISIT_FILE_NAME);
    if( 0 == strlen(visit_file_name_local)){
        strncat(fileinfo->name,"index.html",VISIT_FILE_NAME);
    }
    else{
        strncat(fileinfo->name,visit_file_name_local,VISIT_FILE_NAME);
    }
    if( FILE_NOT_EXIST == is_exist(fileinfo->name)){
        strncpy(fileinfo->name,"www/404.html",VISIT_FILE_NAME);
    }
    print_log(DEBUG, "The required file name is: %s\n\n",fileinfo->name);

    fileinfo->type = get_file_type(fileinfo->name);
    fileinfo->size = get_file_size(fileinfo->name); 
}


void onAccept(int sockfd)
{
    char receive_buffer[RECEIVE_BUFFER_SIZE]={0,};
    recv(sockfd,receive_buffer,RECEIVE_BUFFER_SIZE,0);
    print_log(DEBUG, "%s\n",receive_buffer);

    interceptor(receive_buffer, sizeof(receive_buffer));

    uufile_t req_file_info;
    init_file_info(&req_file_info);
    get_requested_file_info(receive_buffer,&req_file_info);

    int need_buffer_size = SEND_BUFFER_SIZE;
    if(( FILE_TEXT == req_file_info.type) || (FILE_BINARY == req_file_info.type)){
        need_buffer_size = req_file_info.size;
    }

    char *send_buffer=(char *)malloc((need_buffer_size)*sizeof(char));
    if(send_buffer != NULL){
        memset(send_buffer,0,need_buffer_size);
        if( OP_SUCCESS == handle_request_file(&req_file_info,send_buffer,SEND_BUFFER_SIZE)){
            send(sockfd,send_buffer,req_file_info.size,0);
        }
        free(send_buffer);
    }
}

void serve(int sockfd)
{
    int clfd,status;
    pid_t pid;
    for(;;){
        clfd = accept(sockfd,NULL,NULL);
        print_log(DEBUG, "Socket is accepted:\n");
        if(clfd < 0){
            print_log(ERROR, "yebserver: accept error: %s\n",strerror(errno));
            exit(EXIT_ERROR);
        }
        if( (pid = fork()) < 0){
            print_log(ERROR, "yebserver: fork error: %s\n",strerror(errno));
        }
        else if( 0 == pid){
            onAccept(clfd);
            close(clfd) ;
        }
        else{
            close(clfd);
            waitpid(pid,&status,0);
        }
    }
}


int main(int argc,char *argv[])
{
    char fname[] = "main";

    init_log(DEBUG, /*log mask*/
        true, /* print to stderr */            
        "yeb.log", /* log file name */
        10, /* max size (by MB) per file */
        10  /* rotation number */
    );

    print_log(INFO, "%s: Starts up...\n", fname);   

    struct addrinfo *ailist,*aip;
    struct addrinfo hint;
    int             sockfd,err;

    hint.ai_flags       = AI_PASSIVE;
    hint.ai_family      = 0;
    hint.ai_socktype    = SOCK_STREAM;
    hint.ai_protocol    = 0;
    hint.ai_addrlen     = 0;
    hint.ai_canonname   = NULL;
    hint.ai_addr        = NULL;
    hint.ai_next        = NULL;

    if( (err = getaddrinfo(NULL,"www",&hint,&ailist)) != 0 ){
        print_log(ERROR, "yebserver getaddrinfo error: %s\n",gai_strerror(errno));
        exit(EXIT_ERROR);
    }

    print_log(INFO, "%s: Got addr info.\n", fname);   
    
    for(aip = ailist;aip != NULL;aip=aip->ai_next){
        if((sockfd = initserver(SOCK_STREAM,aip->ai_addr,aip->ai_addrlen,QLEN)) >= 0){
            serve(sockfd);
            exit(EXIT_NORMAL);
        }
    }

    print_log(ERROR, "%s: Cannot start web server.\n", fname);   
    exit(EXIT_ERROR);
}
