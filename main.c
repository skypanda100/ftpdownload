#include <stdio.h>
#include <stdlib.h>
#include "def.h"
#include "config.h"
#include "curlftp.h"

int main(int argc,char **argv)
{
    if(argc != 2)
    {
        printf("wrong arguments!!\n");
        exit(1);
    }

    conf cf;
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
    #endif
        // find newest files
        char **newest_file_ptr_ptr = NULL;
        int newest_file_count = 0;
        int ret = get_newest_files(cf.src_dir, cf.user_pwd, &newest_file_ptr_ptr, &newest_file_count);

//        for(int i = 0;i < newest_file_count;i++)
//        {
//            printf("%s\n", newest_file_ptr_ptr[i]);
//        }
        printf("newest files count: %d\n", newest_file_count);

        if(ret == 0)
        {
            // diff and download
            if(!is_first)
            {
                diff_and_download(&cf, newest_file_ptr_ptr, newest_file_count, last_newest_file_ptr_ptr, last_newest_file_count);
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
        } else {
            printf("some errors happened in listing files!\n");
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
    }
    #endif

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
