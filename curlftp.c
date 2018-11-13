//
// Created by root on 8/9/18.
//
#include "curlftp.h"

const char *TMP_SUFFIX_PTR = "tmp";

int make_dir(const char *path_ptr)
{
    char str[1024] = {0};
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

int file_list_write(void *buffer, size_t size, size_t nmemb, void *stream)
{
    ftp_file_list *out = (ftp_file_list *)stream;

    size_t old_len = out->list_ptr == NULL ? 0 : strlen(out->list_ptr);
    size_t add_len = size * nmemb;
    out->list_ptr = (char *)realloc(out->list_ptr, old_len + add_len + 1);
    strncpy(out->list_ptr + old_len, buffer, add_len);
    out->list_ptr[old_len + add_len] = 0;

    return nmemb;
}

int file_write(void *buffer, size_t size, size_t nmemb, void *stream)
{
    ftp_file *out = (ftp_file *)stream;
    if(out && !out->fp)
    {
        out->fp = fopen(out->path_ptr, "wb");
        if(!out->fp)
        {
            return -1;
        }
    }
    return fwrite(buffer, size, nmemb, out->fp);
}

void curl_file_list(const char *path_ptr, const char *user_pwd_ptr, ftp_file_list *file_list_ptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, path_ptr);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd_ptr);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "LIST");
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file_list_ptr);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_list_write);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

void curl_file(const char *path_ptr, const char *user_pwd_ptr, ftp_file *file_ptr)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, path_ptr);
    curl_easy_setopt(curl, CURLOPT_USERPWD, user_pwd_ptr);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file_ptr);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, file_write);
//    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if(file_ptr->fp)
    {
        fclose(file_ptr->fp);
    }
    curl_global_cleanup();

    char new_name[1024] = {0};
    strncpy(new_name, file_ptr->path_ptr, strlen(file_ptr->path_ptr) - strlen(TMP_SUFFIX_PTR) - 1);
    rename(file_ptr->path_ptr, new_name);
}

void get_newest_files(const char *path_ptr, const char *user_pwd_ptr, char ***newest_file_ptr_ptr_ptr, int *newest_file_count_ptr)
{
    char *split_line_ptr = "\n";
    char *split_space_ptr = " ";
    ftp_file_list file_info_list = {NULL};

    curl_file_list(path_ptr, user_pwd_ptr, &file_info_list);
    char *file_info_tmp_ptr = file_info_list.list_ptr;
    char *file_info_ptr = NULL;
    while(file_info_ptr = strsep(&file_info_tmp_ptr, split_line_ptr))
    {
        if(file_info_ptr != NULL && strlen(file_info_ptr) > 0)
        {
            if(file_info_ptr[0] == 'd')
            {
                char *dir_info_tmp_ptr = file_info_ptr;
                char *dir_info_ptr = NULL;
                char *dir_name_ptr = NULL;
                while(dir_info_ptr = strsep(&dir_info_tmp_ptr, split_space_ptr))
                {
                    if(dir_name_ptr != NULL)
                    {
                        free(dir_name_ptr);
                    }
                    dir_name_ptr = strdup(dir_info_ptr);
                }
                if(dir_name_ptr != NULL)
                {
                    if(!(strcmp(dir_name_ptr, ".") == 0 || strcmp(dir_name_ptr, "..") == 0))
                    {
                        char sub_dir_path[1024] = {0};
                        sprintf(sub_dir_path, "%s/%s/", path_ptr, dir_name_ptr);
                        get_newest_files(sub_dir_path, user_pwd_ptr, newest_file_ptr_ptr_ptr, newest_file_count_ptr);
                    }

                    free(dir_name_ptr);
                }
            }
            else
            {
                char *sub_file_info_tmp_ptr = file_info_ptr;
                char *sub_file_info_ptr = NULL;
                char *file_name_ptr = NULL;
                while(sub_file_info_ptr = strsep(&sub_file_info_tmp_ptr, split_space_ptr))
                {
                    if(file_name_ptr != NULL)
                    {
                        free(file_name_ptr);
                    }
                    file_name_ptr = strdup(sub_file_info_ptr);
                }

                if(file_name_ptr != NULL)
                {
                    char *newest_file_ptr = (char *)malloc(1024);
                    memset(newest_file_ptr, 0, 1024);
                    sprintf(newest_file_ptr, "%s/%s", path_ptr, file_name_ptr);
                    (*newest_file_count_ptr)++;

                    *newest_file_ptr_ptr_ptr = realloc(*newest_file_ptr_ptr_ptr, sizeof(char *) * (*newest_file_count_ptr));
                    (*newest_file_ptr_ptr_ptr)[(*newest_file_count_ptr) - 1] = newest_file_ptr;
                    free(file_name_ptr);
                }
            }
        }
    }

    if(file_info_list.list_ptr != NULL)
    {
        free(file_info_list.list_ptr);
        file_info_list.list_ptr = NULL;
    }
}

void diff_and_download(const conf *conf_ptr, char **newest_file_ptr_ptr, int newest_file_count, char **last_newest_file_ptr_ptr, int last_newest_file_count)
{
    char local_dir[1024] = {0};
    char dir[1024] = {0};
    char path[1024] = {0};
    for(int i = 0;i < newest_file_count;i++)
    {
        char *newest_file_ptr = newest_file_ptr_ptr[i];
        int is_exist = 0;
        for(int j = 0;j < last_newest_file_count;j++)
        {
            if(strcmp(last_newest_file_ptr_ptr[j], newest_file_ptr) == 0)
            {
                is_exist = 1;
                break;
            }
        }
        if(!is_exist)
        {
            int j = strlen(newest_file_ptr) - 1;
            for(;j >= 0;j--)
            {
                if(newest_file_ptr[j] == '/')
                {
                    break;
                }
            }
            memset(local_dir, 0, sizeof(local_dir));
            memset(dir, 0, sizeof(dir));
            strncpy(dir, newest_file_ptr, j);
            sprintf(local_dir, "%s/%s", conf_ptr->dst_dir, dir + strlen(conf_ptr->src_dir));
            if(make_dir(local_dir) == 0)
            {
                memset(path, 0, sizeof(path));
                sprintf(path, "%s/%s.%s", conf_ptr->dst_dir, newest_file_ptr + strlen(conf_ptr->src_dir), TMP_SUFFIX_PTR);
                ftp_file file = {
                        path,
                        NULL
                };
                curl_file(newest_file_ptr, conf_ptr->user_pwd, &file);
            }
            #ifdef TEST
            newest_file_count = 0;
            #endif
        }
    }
}