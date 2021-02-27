/*
 * =====================================================================================
 *
 *       Filename:  fileutility.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月17日 19时21分27秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "fileutility.h"

exist_t is_exist(const char *file_name)
{
    exist_t re = FILE_NOT_EXIST;
    if( 0 == access(file_name,F_OK)){
        re = FILE_EXIST;
    }
    return re;
}

file_t get_file_type(const char *filename)
{
    file_t res = FILE_TEXT;

    if(strstr(filename,".gif")   || \
       strstr(filename,".jpg") || \
       strstr(filename,".png") || \
       strstr(filename,".doc") || \
       strstr(filename,".xls") || \
       strstr(filename,".xlsx") || \
       strstr(filename,".docx")  ){
           res = FILE_BINARY;
    }
    else if(strstr(filename,".pl") || \
            strstr(filename,".pr")){
           res = FILE_PERL;
    }
    else if (strstr(filename,".py")) {
        res = FILE_PYTHON;
    }
    return res;
}

op_t read_file(uufile_t *file_info,char *buffer)
{
    
    FILE *fp=NULL;
    if( FILE_BINARY  == file_info->type){
        fp = fopen(file_info->name,"rb");
    }
    else{
        fp = fopen(file_info->name,"r");
    }
    if( NULL == fp){
        fclose(fp);
        return OP_FAIL;
    }
    //assume buffer size is more than file size
    fread(buffer,file_info->size,1,fp);
    fclose(fp);
    return OP_SUCCESS;
}

void init_file_info(uufile_t *file_info)
{
    memset(file_info->name,0,NAME_STRING_MAX);
    file_info->type = FILE_TEXT;
    file_info->size = 0;
    memset(file_info->argv,0,ARGV_STRING_MAX);
}

int get_file_size(const char *filename)
{
    struct stat buf;
    stat(filename,&buf);
    return buf.st_size;
}

op_t call_interpreter (const char *interpreter, uufile_t *file_info,char *send_buffer,int buffer_size)
{
    op_t res = OP_FAIL;
    char *cmd = (char *)malloc(CMD_STRING_MAX*sizeof(char));
    memset(cmd,0,CMD_STRING_MAX);
    sprintf(cmd,"%s %s %s", interpreter, file_info->name, file_info->argv);

    FILE *fp = popen(cmd,"r");

    if(NULL == fp){
        res = OP_FAIL;
    }
    else{
        fread(send_buffer,buffer_size,1,fp);
        file_info->size = strlen(send_buffer);
        res = OP_SUCCESS;
    }
    pclose(fp);
    free(cmd);
    return res;    
}

op_t handle_request_file(uufile_t *file_info,char *send_buffer,int buffer_size)
{
    op_t res = OP_FAIL;
    if( FILE_PERL == file_info->type){
        if( OP_SUCCESS == call_interpreter("perl", file_info,send_buffer,buffer_size)){
            res = OP_SUCCESS;
        }
    } else if (FILE_PYTHON == file_info->type) {
        if( OP_SUCCESS == call_interpreter("python3", file_info,send_buffer,buffer_size)){
            res = OP_SUCCESS;
        }
    }
    else{
        if( OP_SUCCESS == read_file(file_info,send_buffer)){
            res = OP_SUCCESS;
        }
    }
    return res;
}

