/*
 *	Power BY Bill Lonely
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
/*work state*/
#define RUNNING 0
#define READY 1
#define IDLE 2

typedef struct worker
{
	void *(* process)(void *arg);/*回调函数*/
	void *arg;/*参数*/
	struct worker *next;
}CThread_worker;

typedef struct 
{
    pthread_t thread_id;
	int state;
} ThreadUnit;

typedef struct 
{
    /*任务队列*/
    CThread_worker *work_queue_head; /* 任务队列头部指针*/
    CThread_worker *work_quequ_rear; /* 任务队列尾部指针*/

    int shutdown; /*是否要销毁线程池*/
    ThreadUnit *thread_unite_list;
    /*最大线程数*/
    int max_thread_num;
    
    int cur_queue_size;
    
    /*线程同步控制*/
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

}CThread_pool;

int pool_init(int max_thread_num);/*线程池初始化*/
void *thread_routine(void *arg);/*线程工作体*/
int pool_destroy(void);/*销毁线程池*/
int pool_add_worker(void *(*process)(void *arg), void *arg );/*添加工作*/
int pool_check_state(void);
int pool_destroy_force(void);
#endif
