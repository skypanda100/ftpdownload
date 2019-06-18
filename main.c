#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "def.h"
#include "config.h"
#include "curlftp.h"
#include "log.h"

conf cf;

int main(int argc,char **argv)
{
    if(argc != 2)
    {
        printf("wrong arguments!!\n");
        exit(1);
    }

    memset(&cf, 0, sizeof(conf));
    config(&cf, argv[1]);

    if(strlen(cf.src_dir) == 0)
    {
        printf("src_dir is wrong!!\n");
        exit(1);
    }

    if(strlen(cf.dst_dir) == 0)
    {
        printf("dst_dir is wrong!!\n");
        exit(1);
    }

    if(strlen(cf.user_pwd) == 0)
    {
        printf("user_pwd is wrong!!\n");
        exit(1);
    }

    if(cf.sleep_time <= 0)
    {
        printf("sleep_time must be greater than 0!!\n");
        exit(1);
    }

    if(cf.low_speed_time < 30)
    {
        printf("low_speed_time must be greater than 30!!\n");
        exit(1);
    }

    #ifndef TEST
    daemon(0, 0);
    #endif

    char **last_newest_file_ptr_ptr = NULL;
    int last_newest_file_count = 0;
    #ifndef TEST
    int is_first = 1;
    #else
    int is_first = 0;
    #endif

    #ifndef TEST
    for(;;)
    {
    #else
    for(int loop = 0;loop < 2;loop++)
    {
    #endif
        // find newest files
        char **newest_file_ptr_ptr = NULL;
        int newest_file_count = 0;
        char src_dir[1024] = {0};
        char date[32] = {0};
        if(strlen(cf.date_dir) > 0)
        {
            time_t t;
            time(&t);
            strftime(date, sizeof(date), cf.date_dir, localtime(&t));
            sprintf(src_dir, "%s/%s", cf.src_dir, date);
        }
        else
        {
            strcpy(src_dir, cf.src_dir);
        }
        LOG("src_dir: %s", src_dir);
        int ret = get_newest_files(cf.low_speed_time, src_dir, cf.user_pwd, &newest_file_ptr_ptr, &newest_file_count);
        LOG("newest files count: %d", newest_file_count);
        if(ret == 0)
        {
            // diff and download
            if(!is_first)
            {
                diff_and_download(&cf, newest_file_ptr_ptr, newest_file_count, last_newest_file_ptr_ptr,
                                  last_newest_file_count);
            }
            is_first = 0;

            if(last_newest_file_ptr_ptr != NULL)
            {
                for(int i = 0;i < last_newest_file_count;i++)
                {
                    if(last_newest_file_ptr_ptr[i] != NULL)
                    {
                        free(last_newest_file_ptr_ptr[i]);
                    }
                }
                free(last_newest_file_ptr_ptr);
                last_newest_file_ptr_ptr = NULL;
            }

            last_newest_file_ptr_ptr = newest_file_ptr_ptr;
            last_newest_file_count = newest_file_count;
        }
        else
        {
            LOG("some errors happened in listing files!");
            if(newest_file_ptr_ptr != NULL)
            {
                for(int i = 0;i < newest_file_count;i++)
                {
                    if(newest_file_ptr_ptr[i] != NULL)
                    {
                        free(newest_file_ptr_ptr[i]);
                    }
                }
                free(newest_file_ptr_ptr);
                newest_file_ptr_ptr = NULL;
            }
        }

    #ifndef TEST
        sleep(cf.sleep_time);
    #endif
    }

    if(last_newest_file_ptr_ptr != NULL)
    {
        for(int i = 0;i < last_newest_file_count;i++)
        {
            if(last_newest_file_ptr_ptr[i] != NULL)
            {
                free(last_newest_file_ptr_ptr[i]);
            }
        }
        free(last_newest_file_ptr_ptr);
        last_newest_file_ptr_ptr = NULL;
    }

    return 0;
}
