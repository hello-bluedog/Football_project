/*************************************************************************
	> File Name: udp_epoll.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 04:40:42 PM CST
 ************************************************************************/

#include "head.h"

extern struct User *rteam;
extern struct User *bteam;
extern int repollfd, bepollfd;
extern int port;
void send_all(struct ChatMsg *msg)
{
    for (int i = 0; i < MAX; i++) {
        if (bteam[i].online) {
            send(bteam[i].fd, (void *)msg, sizeof(struct ChatMsg), 0);    
        }
        if (rteam[i].online) {
            send(rteam[i].fd, (void *)msg, sizeof(struct ChatMsg), 0);    
        }   
    }
}

void send_to(char *to, struct ChatMsg *msg, int fd)
{
    int flag = 0;
    for (int i = 0; i < MAX; i++) { //需要校正i
        if (rteam[i].online && !strcmp(to, rteam[i].name)) {
            send(rteam[i].fd, msg, sizeof(struct ChatMsg), 0);
            flag = 1;
            break;
        }
        if (bteam[i].online && !strcmp(to, bteam[i].name)) {
            send(bteam[i].fd, msg, sizeof(struct ChatMsg), 0);
            flag = 1;
            break;             
        }
    }
    if (!flag) {
        memset(msg->msg, 0, sizeof(msg->msg));
        strcpy(msg->name, to);
        sprintf(msg->msg, "用户 %s 不在线，或用户名错误！", to);
        msg->type = CHAT_SYS;
        send(fd, msg, sizeof(struct ChatMsg), 0);    
    }
}

void add_event_ptr(int epollfd, int fd, int events, struct User* user) {
    struct epoll_event ev;
    ev.events = events;
    ev.data.ptr = user;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
}

void del_event(int epollfd, int fd){
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

int find_sub(struct User *team){
    for(int i = 0; i < MAX; i++){
        if(!team[i].online) return i;
    }
    return -1;
}
int udp_connect(struct sockaddr_in *client){
    int sockfd;
    if((sockfd = socket_create_udp(port)) < 0){
        perror("socket_udp");
        return -1;
    }
    if(connect(sockfd, (struct sockaddr *)client, sizeof(struct sockaddr)) < 0){
        perror("connect()");
        return -1;
    }
    return sockfd;
}
int check_online(struct LogRequest *request){
    for(int i =0; i < MAX; i++){
        if(rteam[i].online && !strcmp(request->name, rteam[i].name)) return 1;
        if(bteam[i].online && !strcmp(request->name, bteam[i].name)) return 1;
    }
    return 0;
}
int udp_accept(int fd, struct User *user){
    int new_fd, retval;
    struct sockaddr_in client;
    struct LogRequest request;
    struct LogResponse response;
    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));
    socklen_t len = sizeof(client);
    
    int ret = recvfrom(fd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&client, &len);

    if (ret != sizeof(request)){
        response.type = 1;
        strcpy(response.msg, "Login failed with Data errora!\n");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }
    if (check_online(&request)){
        response.type = 1;
        strcpy(response.msg, "You are Already Login in!");
        sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);
        return -1;
    }
    response.type = 0;
    strcpy(response.msg, "Login Success.Enjoy yourself!\n");
    sendto(fd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&client, len);

    if (request.team) {
        DBG(GREEN"INFO"NONE" : BLUE %s login on %s:%d <%s>\n", request.name, inet_ntoa(client.sin_addr), ntohs(client.sin_port), request.msg);
    } else {
        DBG(GREEN"INFO"NONE" : BLUE %s login on %s:%d<%s>\n", request.name, inet_ntoa(client.sin_addr), ntohs(client.sin_port), request.msg);
    }

    strcpy(user->name, request.name);
    user->team = request.team;
    new_fd = udp_connect(&client);
    user->fd = new_fd;
    return new_fd;
}
void add_to_sub_reactor(struct User *user){
    struct User *team = (user->team ? bteam : rteam);
    struct epoll_event ev;
    int sub = find_sub(team);
    if (sub < 0){
        fprintf(stderr, "Full Team!\n");
        return;
    }

    team[sub] = *user;
    team[sub].online = 1;
    team[sub].flag = 10;
    DBG(L_RED"sub = %d, name = %s\n", sub, user->name);
    if (user->team){
        add_event_ptr(bepollfd, user->fd, EPOLLIN | EPOLLET, user);
    } else {
        add_event_ptr(repollfd, user->fd, EPOLLIN | EPOLLET, user);
    }
    return;
}
