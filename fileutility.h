/*
 * =====================================================================================
 *
 *       Filename:  fileutility.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012年02月17日 19时24分04秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#ifndef FILEUTILITY
#define FILEUTILITY

#define NAME_STRING_MAX 100
#define ARGV_STRING_MAX 100
#define CMD_STRING_MAX 1024*100 

typedef enum {
    FILE_NOT_EXIST   = 1,
    FILE_EXIST       = 2
}exist_t;

typedef enum {
    FILE_TEXT   =   1,
    FILE_BINARY =   2,
    FILE_PERL   =   3,
    FILE_PHP    =   4,
    FILE_JSP    =   5,
    FILE_PYTHON =   6
}file_t;

typedef enum {
    OP_SUCCESS    = 1,
    OP_FAIL       = 2
}op_t;


typedef struct{
    char    name[NAME_STRING_MAX];
    file_t  type;
    int     size; 
    char    argv[ARGV_STRING_MAX];
}uufile_t;

extern exist_t is_exist(const char *file_name);
extern file_t get_file_type(const char *filename);
extern op_t read_file(uufile_t *file_info,char *buffer);
extern void init_file_info(uufile_t *file_info);
extern int get_file_size(const char *filename);
extern op_t handle_request_file(uufile_t *file_info,char *send_buffer,int buffer_size);
#endif
