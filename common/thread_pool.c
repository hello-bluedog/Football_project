/*************************************************************************
	> File Name: thread_pool.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 02:49:18 PM CST
 ************************************************************************/

#include "head.h"
extern int repollfd, bepollfd;
extern pthread_mutex_t rmutex; 
extern pthread_mutex_t bmutex;
void do_work(struct User *user){
    struct ChatMsg msg, r_msg;
    bzero(&msg, sizeof(msg));
    recv(user->fd, (void *)&msg,sizeof(msg), 0);
    if(msg.type & CHAT_WALL){
        printf("<%s> ~ %s\n", msg.name, msg.msg);
        struct ChatMsg r_msg;
        bzero(&r_msg, sizeof(r_msg));
        r_msg.type = CHAT_SYS;  //需要校正
        strcpy(msg.name, user->name);
        sprintf(r_msg.msg, "你的好友 "RED"%s"NONE" 上线了，快打个招呼吧！",user->name);
        send_all(&r_msg);
        send_all(&msg);        
    } else if(msg.type & CHAT_MSG){
        char to[20] = {0};
        memset(to, 0, sizeof(to));
        int i = 1;
        for (; i <= 21; i++) {
            if (msg.msg[i] == ' ') {
                break;
            }            
        }
        if (msg.msg[i] != ' ' || msg.msg[0] != '@') {
            memset(&r_msg, 0, sizeof(r_msg));
            r_msg.type = CHAT_SYS;
            sprintf(r_msg.msg, "私聊格式错误！");
            send(user->fd, (void *)&r_msg, sizeof(r_msg), 0);  
        } else {
            msg.type = CHAT_MSG;
            strcpy(msg.name, user->name);
            strncpy(to, msg.msg + 1, i - 1);
            send_to(to, &msg, user->fd);        
        }    
    } else if(msg.type & CHAT_FIN){
        bzero(msg.msg, sizeof(msg.msg));
        msg.type = CHAT_SYS;
        sprintf(msg.msg, "注意：我们的好朋友 %s 要下线了！\n", user->name);
        strcpy(msg.name, user->name);
        send_all(&msg);
        if (user->team) {
            pthread_mutex_lock(&bmutex);        
        } else {
            pthread_mutex_lock(&rmutex);        
        }
        user->online = 0;
        int epollfd = user->team ? bepollfd : repollfd;
        del_event(epollfd, user->fd);
        if(user->team) {
            pthread_mutex_unlock(&bmutex);        
        } else {
            pthread_mutex_unlock(&rmutex);        
        }
        printf(GREEN"Server Info"NONE" : %s logout!\n", user->name);
        close(user->fd);
    } 

void task_queue_init(struct task_queue *taskQueue, int sum, int epollfd){
    taskQueue->sum = sum;
    taskQueue->epollfd = epollfd;
    taskQueue->team = calloc(sum, sizeof(void *));
    taskQueue->head = taskQueue->tail = 0;
    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);
}

void task_queue_push(struct task_queue *taskQueue, struct User *user){
    pthread_mutex_lock(&taskQueue->mutex);
     taskQueue->team[taskQueue->tail] = user;
    if(++taskQueue->tail == taskQueue->sum){
        DBG(L_GREEN"Thread Pool"NONE" : Task Queue End\n");
        taskQueue->tail = 0;
    }
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);
}


struct User *task_queue_pop(struct task_queue *taskQueue){
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->head == taskQueue->tail){
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }

    struct User *user = taskQueue->team[taskQueue->head];
    if (++taskQueue->head == taskQueue->sum){
        DBG(L_GREEN"Thread Pool"NONE" : Task Queue End\n");
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return user;
}

void *thread_run(void *arg){
    pthread_detach(pthread_self());
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while(1){
        struct User *user = task_queue_pop(taskQueue);
        do_work(user);
    }
}
