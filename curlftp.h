//
// Created by root on 8/9/18.
//

#ifndef FTPDOWNLOAD_CURLFTP_H
#define FTPDOWNLOAD_CURLFTP_H

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "def.h"

int make_dir(const char *path_ptr);
int file_list_write(void *buffer, size_t size, size_t nmemb, void *stream);
int file_write(void *buffer, size_t size, size_t nmemb, void *stream);
CURLcode curl_file_list(const char *path_ptr, const char *user_pwd_ptr, ftp_file_list *file_list_ptr);
CURLcode curl_file(const char *path_ptr,  const char *user_pwd_ptr, ftp_file *file_ptr);
int get_newest_files(const char *path_ptr, const char *user_pwd_ptr, char ***newest_file_ptr_ptr_ptr, int *newest_file_count_ptr);
void diff_and_download(const conf *conf_ptr, char **newest_file_ptr_ptr, int newest_file_count, char **last_newest_file_ptr_ptr, int last_newest_file_count);

#endif //FTPDOWNLOAD_CURLFTP_H
