//
// Created by zhengdongtian on 4/1/19.
//
#include "log.h"

extern conf cf;

static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static int make_dir(const char *path_ptr)
{
    char str[PATH_MAX] = {0};
    strcpy(str, path_ptr);
    int len = strlen(str);

    for(int i = 0;i < len;i++)
    {
        if(str[i] == '/')
        {
            str[i] = '\0';
            if(strlen(str) == 0)
            {
                str[i] = '/';
                continue;
            }
            if(access(str, F_OK) != 0)
            {
                if(mkdir(str, 0777) != 0)
                {
                    return -1;
                }
            }
            str[i] = '/';
        }
    }

    if(len > 0 && access(str, F_OK) != 0)
    {
        if(mkdir(str, 0777) != 0)
        {
            return -1;
        }
    }

    return 0;
}

void logger(const char *source_ptr, size_t line, const char *format_ptr, ...)
{
    char *dir_ptr = cf.log_dir;

    if(strlen(dir_ptr) > 0)
    {
        pthread_mutex_lock(&log_mutex);

        if(make_dir(dir_ptr) != 0)
        {
            fprintf(stderr, "make dirs failed!\n");
            return;
        }

        char file_path[PATH_MAX] = {0};
        FILE *fp = NULL;
        char *buf_ptr = NULL;
        time_t t;
        struct tm *tm_ptr = NULL;

        time(&t);
        tm_ptr = localtime(&t);
        sprintf(file_path, "%s/%04d-%02d-%02d.log",
                dir_ptr,
                tm_ptr->tm_year + 1900,
                tm_ptr->tm_mon + 1,
                tm_ptr->tm_mday);

        if(!(fp = fopen(file_path, "a+")))
        {
            pthread_mutex_unlock(&log_mutex);
            return;
        }

        va_list list;
        va_start(list, format_ptr);
        vasprintf(&buf_ptr, format_ptr, list);
        va_end(list);

        if(!buf_ptr)
        {
            pthread_mutex_unlock(&log_mutex);
            return;
        }

        fprintf(fp, "[%02d:%02d:%02d %s %ld] %s\n",
                tm_ptr->tm_hour,
                tm_ptr->tm_min,
                tm_ptr->tm_sec,
                source_ptr,
                line,
                buf_ptr);

        free(buf_ptr);
        fsync(fileno(fp));
        fclose(fp);

        pthread_mutex_unlock(&log_mutex);
    }
}
