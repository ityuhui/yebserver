/*
 * =====================================================================================
 *
 *       Filename:  httprotocol.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2012-02-17 19:28:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdlib.h>
#include "httprotocol.h"

void get_visit_file_name_and_argv(const char *s,char *visit_name,char *visit_argv,int argv_buffer_size)
{
    char *p = index(s,' ') + 2;
    char *q = index(p,' ');
    int n = abs(q-p); 
    int i;
    for(i=0;i<n;i++){
        visit_name[i]=p[i];
    }
    
    char *i_arg = index(visit_name,'?');
    if(i_arg != NULL){
        strncpy(visit_argv,i_arg + 1,argv_buffer_size);
        *i_arg = '\0';
    }

}
