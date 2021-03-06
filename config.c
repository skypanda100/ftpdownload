//
// Created by root on 8/9/18.
//
#include "config.h"

static char *key_src_dir = "src_dir";
static char *key_dst_dir = "dst_dir";
static char *key_user_pwd = "user_pwd";
static char *key_sleep_time = "sleep_time";
static char *key_low_speed_time = "low_speed_time";
static char *key_log_dir = "log_dir";
static char *key_date_dir = "date_dir";

static char *l_trim(char *output, const char *input)
{
    assert(input != NULL);
    assert(output != NULL);
    assert(output != input);
    for(;*input != '\0' && isspace(*input);++input)
    {
        ;
    }
    return strcpy(output, input);
}

static char * a_trim(char * output, const char * input)
{
    char *p = NULL;
    assert(input != NULL);
    assert(output != NULL);
    l_trim(output, input);
    for(p = output + strlen(output) - 1;p >= output && isspace(*p); --p)
    {
        ;
    }
    *(++p) = '\0';
    return output;
}


void config(conf *conf_ptr, char *conf)
{
    char key[32] = {0};
    char val_src_dir[1024] = {0};
    char val_dst_dir[1024] = {0};
    char val_user_pwd[128] = {0};
    char val_sleep_time[12] = {0};
    char val_low_speed_time[12] = {0};
    char val_log_dir[1024] = {0};
    char val_date_dir[128] = {0};

    char *buf, *c;
    char buf_i[1024], buf_o[1024];
    FILE *fp;
    if((fp=fopen( conf,"r" )) == NULL)
    {
        printf("openfile [%s] error [%s]\n", conf, strerror(errno));
        exit(1);
    }
    fseek(fp, 0, SEEK_SET);

    while(!feof(fp) && fgets(buf_i, 1024, fp) != NULL)
    {
        l_trim(buf_o, buf_i);
        if(strlen(buf_o) <= 0)
            continue;
        buf = buf_o;

        if(buf[0] == '#')
        {
            continue;
        }
        else
        {
            if((c = strchr(buf, '=')) == NULL)
                continue;
            memset(key, 0, sizeof(key));
            sscanf(buf, "%[^= \t]", key);
            if(strcmp(key, key_src_dir) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_src_dir);
                char *val_o = (char *)malloc(strlen(val_src_dir) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_src_dir) + 1);
                    a_trim(val_o, val_src_dir);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_src_dir, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
            else if(strcmp(key, key_dst_dir) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_dst_dir);
                char *val_o = (char *)malloc(strlen(val_dst_dir) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_dst_dir) + 1);
                    a_trim(val_o, val_dst_dir);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_dst_dir, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
            else if(strcmp(key, key_user_pwd) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_user_pwd);
                char *val_o = (char *)malloc(strlen(val_user_pwd) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_user_pwd) + 1);
                    a_trim(val_o, val_user_pwd);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_user_pwd, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
            else if(strcmp(key, key_sleep_time) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_sleep_time);
                char *val_o = (char *)malloc(strlen(val_sleep_time) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_sleep_time) + 1);
                    a_trim(val_o, val_sleep_time);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_sleep_time, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
            else if(strcmp(key, key_low_speed_time) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_low_speed_time);
                char *val_o = (char *)malloc(strlen(val_low_speed_time) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_low_speed_time) + 1);
                    a_trim(val_o, val_low_speed_time);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_low_speed_time, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
            else if(strcmp(key, key_log_dir) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_log_dir);
                char *val_o = (char *)malloc(strlen(val_log_dir) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_log_dir) + 1);
                    a_trim(val_o, val_log_dir);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_log_dir, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
            else if(strcmp(key, key_date_dir) == 0)
            {
                sscanf(++c, "%[^\n\r]", val_date_dir);
                char *val_o = (char *)malloc(strlen(val_date_dir) + 1);
                if(val_o != NULL){
                    memset(val_o, 0, strlen(val_date_dir) + 1);
                    a_trim(val_o, val_date_dir);
                    if(val_o && strlen(val_o) > 0)
                        strcpy(val_date_dir, val_o);
                    free(val_o);
                    val_o = NULL;
                }
            }
        }
    }
    fclose(fp);

    int src_dir_len = strlen(val_src_dir);
    if(src_dir_len > 0)
    {
        if(val_src_dir[src_dir_len - 1] != '/')
        {
            val_src_dir[src_dir_len] = '/';
            val_src_dir[src_dir_len + 1] = 0;
        }
    }

    strcpy(conf_ptr->src_dir, val_src_dir);
    strcpy(conf_ptr->dst_dir, val_dst_dir);
    strcpy(conf_ptr->user_pwd, val_user_pwd);
    conf_ptr->sleep_time = atoi(val_sleep_time);
    conf_ptr->low_speed_time = atoi(val_low_speed_time);
    strcpy(conf_ptr->log_dir, val_log_dir);
    strcpy(conf_ptr->date_dir, val_date_dir);
}
