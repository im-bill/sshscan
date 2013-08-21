/*Power BY Bill Lonely*/
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <libssh2.h>
#include <time.h>
#include <pthread.h>

#ifndef _SSHSCAN_H_
#define _SSHSCAN_H_

#define FROM_UNKNOWN -1
#define FROM_KEYBOARD 0
#define FROM_FILE 1

#define MAX_LEN_FILENAME 256
#define MAX_LEN_TIME_STR 256


#define MAX_LEN_PATH    2048
typedef  struct _Setting
{
	char *Host_IP;
	char *Host_File;
	char *Username;
	char *User_File;
    char *Password;
    char *Password_File;
    short port;
    int     connect_test_count;   /*连接测试次数*/
    char success_log_filename[MAX_LEN_PATH];  
    char path_log[MAX_LEN_PATH];  /*日志文件路径*/
    pthread_mutex_t success_log_mutex;  /*成功日志文件互斥*/
    pthread_mutex_t setting_mutex;  /*登记线程参数指针互斥*/
    struct password_node **pwd_groups;  /*分组密码数组*/
    int per_pwd_num;     /*每组大小*/
    struct workarg_queue *workarg_list_head ;
    int pwd_group_num;  /*密码分组数量*/
    int thread_num;  /*线程数	*/
}Setting;

struct ip_node
{
    char *ip;
    struct ip_node *next;
};
struct ip_list
{
    struct ip_node *head;
    int count;
};

struct user_node
{
    char *user;
    struct user_node *next;
};
struct user_list
{
    struct user_node *head;
    int count;
};

struct password_node
{
    char *password;
    struct password_node *next;
};

struct password_list
{
    struct password_node *head;
    int count;
};

/*Level 1*/
struct Test_ssh_Arg_by_IP
{
    struct ip_node ip;
    struct user_list users;
    struct password_list passwords;  
    int ret;
    short port;    
    Setting *setting;
};

/*Level 2*/
struct Try_login_arg_by_user
{
  char *ip;
  char *user;
  short port;
  struct password_list *passwords;
  int complete;
  pthread_mutex_t complete_mutex;  /*在free过程中记得要destroy掉*/
  int ret;
  Setting *setting;
};

/*Level 3*/
struct Try_login_arg_by_pwd
{
    struct Try_login_arg_by_user *lastLevArg;
    struct password_node *passwords;
    int ret;
};

struct workarg_queue
{
    void *point;
    int level;
    struct workarg_queue *next;
};

void* ssh_test(void *arg);
int checkSetting(int argc, char **argv, Setting *setting);
void free_setting(Setting *setting);
int analysisSetting(Setting *setting, struct ip_list *IPs, 
        struct user_list *Users, struct password_list *Passwords);
void free_ip_list(struct ip_list *IPs);
void free_user_list(struct user_list *Users);
void free_password_list(struct password_list *Passwords);
void* try_login_pwd(void *arg);
void* try_login_user(void *arg);

int test_connect(char *ip, short port, int test_count);


#endif
