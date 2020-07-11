/*************************************************************************
	> File Name: thread_pool.h
	> Author: jzx
	> Mail: 1790592295@qq.com
	> Created Time: Thu 09 Jul 2020 02:49:26 PM CST
 ************************************************************************/

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
struct task_queue {
    int sum;
    int epollfd;//从反应堆
    struct User **team;//用户数组的地址
    int head;
    int tail;
    pthread_mutex_t mutex;//锁,防止数据竞争
    pthread_cond_t cond;
};
void do_work(struct User *user);
void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd);
void task_queue_push(struct task_queue *taskQueue, struct User *user);
struct User *task_queue_pop(struct task_queue *taskQueue);
void *thread_run(void *arg);
#endif
