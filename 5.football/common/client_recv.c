/*************************************************************************
	> File Name: client_recv.c
	> Author: jzx
	> Mail: 1790592295@qq.com
	> Created Time: Sat 11 Jul 2020 08:15:48 AM CST
 ************************************************************************/

#include "head.h"
extern int sockfd;

void *do_recv(void *arg){
    while(1){
        struct ChatMsg msg;
        bzero(&msg, sizeof(msg));
        int ret = recv(sockfd, (void *)&msg, sizeof(msg), 0);
        if(msg.type & CHAT_WALL){
            printf(""BLUE"%s"NONE" :%s\n", msg.name, msg.msg);
        } else if(msg.type & CHAT_MSG){
            printf(""RED"%s"NONE" :%s\n", msg.name, msg.msg);
        } else if(msg.type & CHAT_SYS){
            printf(YELLOW"Server Info"NONE" : %s\n", msg.msg);
        } else if(msg.type & CHAT_FIN){
            printf(L_RED"Server Info"NONE"Server Down!\n");
            exit(1);
        }
    }
}

