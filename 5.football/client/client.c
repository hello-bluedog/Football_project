/*************************************************************************
	> File Name: client.c
	> Author: jzx
	> Mail: 1790592295@qq.com
	> Created Time: Fri 10 Jul 2020 03:02:45 AM CST
 ************************************************************************/

#include "head.h"

int server_port = 0;
int team = -1;
char server_ip[20] = {0};
char *conf = "./football.conf";
int sockfd = -1;
WINDOW *message_win, *message_sub, *info_win, *info_sub, *input_win, *input_sub;
int msgnum = 0;



void logout(int signum){
    struct ChatMsg msg;
    msg.type = CHAT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    close(sockfd);
    DBG(RED"Bye!\n"NONE);
    exit(0);
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");
    int opt;
    struct LogRequest request;
    struct LogResponse response;
    
    bzero(&request, sizeof(request));
    bzero(&response, sizeof(response));
    while ((opt = getopt(argc, argv, "h:p:t:m:n:")) != -1) {
        switch (opt) {
            case 't':
                request.team = atoi(optarg);
                break;
            case 'h':
                strcpy(server_ip, optarg);
                break;
            case 'p':
                server_port = atoi(optarg);
                break;
            case 'm':
                strcpy(request.msg, optarg);
                break;
            case 'n':
                strcpy(request.name, optarg);
                break;
            default:
                fprintf(stderr, "Usage : %s [-hptmn]!\n", argv[0]);
                exit(1);
        }
    }
        
        if (!server_port) server_port = atoi(get_conf_value(conf, "SERVERPORT"));
        if (!request.team) request.team = atoi(get_conf_value(conf, "TEAM"));
        if (!strlen(server_ip)) strcpy(server_ip, get_conf_value(conf, "SERVERIP"));
        if (!strlen(request.name)) strcpy(request.name, get_conf_value(conf, "NAME"));
        if (!strlen(request.msg)) strcpy(request.msg, get_conf_value(conf, "LOGMSG"));



    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    socklen_t len = sizeof(server);
    
    init_ui();
    if ((sockfd = socket_create_udp(server_port)) < 0) {
        perror("socket_create_udp()");
        exit(1);
    }
    
    sendto(sockfd, (void *)&request, sizeof(request), 0, (struct sockaddr *)&server, len);
    
    struct ChatMsg tmp;
    fd_set rfds;
    struct timeval tv;
    int retval;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    retval = select(sockfd + 1, &rfds, NULL, NULL, &tv);
    if(retval < 0){
        perror("select()");
        exit(1);
    } else if(retval == 0){
        DBG(RED"Error"NONE"The Game Server is out of service!\n ");
        exit(1);
    }
    else{
        int ret = recvfrom(sockfd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&server, &len);
        if(ret != sizeof(response) || response.type == 1){
            DBG(RED"Error"NONE" The Game Server refuse to accept %s\n",response.msg);
            exit(1);
        } 
    }
    strcpy(tmp.msg, response.msg);
    show_message(message_sub, &tmp, 1);
    
    connect(sockfd, (struct sockaddr *)&server, len);
    
/*    char buff[512] = {0};
    sprintf(buff, "jzx");
    send(sockfd, buff, strlen(buff), 0);
*/
    pthread_t recv_t;
    pthread_create(&recv_t, NULL, do_recv, NULL);

    signal(SIGINT, logout);
    
    noecho();
    cbreak;
    while(1){
        send_chat();
    }
    
/*

  //  char buff[512] = {0};
    //sprintf(buff, "hi");
   // send(sockfd, buff, strlen(buff), 0);
   recv(sockfd, buff, sizeof(buff), 0);

        DBG(RED"Server Info"NONE" : %s", buff);
           
    while(1){
            struct ChatMsg msg;
            msg.type = CHAT_WALL;
            printf(RED"Please Input: \n"NONE);
            scanf("%[^\n]s",msg.msg);
            getchar();
            send(sockfd, (void *)&msg, sizeof(msg), 0);
        }

  */  
    return 0;

}

