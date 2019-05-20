//
// Created by root on 8/9/18.
//

#ifndef FTPDOWNLOAD_DEF_H
#define FTPDOWNLOAD_DEF_H

#include <stdio.h>

//#define TEST

typedef struct st_ftp_file_list
{
    char *list_ptr;
}ftp_file_list;

typedef struct st_ftp_file
{
    char *path_ptr;
    FILE *fp;
}ftp_file;

typedef struct st_conf
{
    char src_dir[1024];
    char dst_dir[1024];
    char user_pwd[128];
    int sleep_time;
    int low_speed_time;
}conf;

#endif //FTPDOWNLOAD_DEF_H
