/*************************************************************************
	> File Name: sub_reacter.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 03:43:48 PM CST
 ************************************************************************/

#include "head.h"

void *sub_reactor(void *arg){

    struct task_queue *taskQueue = (struct task_queue *)arg;
    pthread_t *tid = (pthread_t *)calloc(NTHREAD, sizeof(pthread_t));
    for (int i = 0; i < NTHREAD; i++){
        pthread_create(&tid[i], NULL, thread_run, (void *)taskQueue);
    }

    struct epoll_event ev, events[MAX];
    while(1){
        int nfds = epoll_wait(taskQueue->epollfd, events, MAX, -1);
        if(nfds < 0){
            perror("epoll_wait");
            exit(1);
        }
        for (int i = 0; i < nfds; i++){
            struct User *user = (struct User*)events[i].data.ptr;
            if (events[i].events & EPOLLIN){
                task_queue_push(taskQueue, (struct User*)events[i].data.ptr);
            }
        }
    }

    return NULL;
}
