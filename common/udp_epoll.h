/*************************************************************************
	> File Name: udp_epoll.h
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 04:40:53 PM CST
 ************************************************************************/

#ifndef _UDP_EPOLL_H
#define _UDP_EPOLL_H
void send_to(char *to, struct ChatMsg *msg, int fd);
void send_all(struct ChatMsg *msg);
void disp_list(struct User *user);
int udp_accept(int fd, struct User *user);
int udp_connect(struct sockaddr_in *client);
void del_event(int epollfd, int fd);
void add_to_sub_reactor(struct User *user);
#endif
