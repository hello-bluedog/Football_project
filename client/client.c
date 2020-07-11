/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: Wed 08 Jul 2020 03:06:44 PM CST
 ************************************************************************/

#include "head.h"

char *path = "footballd.conf";
int server_port;
int sockfd = -1;
char server_ip[20] = {0};
void logout(int signum){
    struct ChatMsg msg;
    msg.type = CHAT_FIN;
    send(sockfd, (void *)&msg, sizeof(msg), 0);
    close(sockfd);
    DBG(RED"BYE!\n");
    exit(0);
}
int main(int argc, char **argv){
    char opt;
    struct LogRequest request;
    struct LogResponse response;
    memset(&request, 0, sizeof(request));
    memset(&response, 0, sizeof(response));
    while((opt = getopt(argc, argv, "h:p:t:m:n:")) != -1){
        switch (opt){
            case 'h': strcpy(server_ip, optarg);break;
            case 'p':server_port = atoi(optarg);break;
            case 'n':strcpy(request.name, optarg);break;
            case 't': request.team = atoi(optarg);break;
            case 'm':strcpy(request.msg, optarg);break;
            default :
            fprintf(stderr, "Usage : %s [-hptmn]\n", argv[0]);
            exit(1);
        }
        
    }

    if(!strlen(request.msg)) strcpy(request.msg, get_conf_value(path,"LOGMSG"));
    if(!request.team) request.team = atoi(get_conf_value(path,"TEAM"));
    if(!server_port) server_port = atoi(get_conf_value(path,"SERVERPORT"));
    if(!strlen(request.name)) strcpy(request.name,get_conf_value(path,"NAME"));
    if(!strlen(server_ip)) strcpy(server_ip,get_conf_value(path,"SERVER_IP"));
    DBG("<"GREEN"Conf Show"NONE"> : server_ip = %s, port = %d, team = %s, name = %s msg : %s\n", server_ip, server_port, request.team ?  "BLUE" : "RED", request.name, request.msg);    

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr(server_ip);

    socklen_t len = sizeof(server);

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket_udp()");
        exit(1);
    }
    sendto(sockfd, (void *)&request, sizeof(request), 0 ,(struct sockaddr *)&server, len);

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    int retval = select(sockfd + 1, &rfds, NULL, NULL, &tv);
    if(retval < 0){
        DBG(RED"No response ! log out!\n");
        exit(1);
    } else if (retval){
       int ret = recvfrom(sockfd, (void *)&response, sizeof(response), 0, (struct sockaddr *)&server, &len);
        if (ret == -1 || response.type == 1 || ret != sizeof(response)){
            DBG(RED"server refuse to accept %s!\n", response.msg);
            exit(1);
        } 
    } else {
        DBG(RED"Error"NONE"The game Server is out of service!\n");
        exit(1);
        }

        if (connect(sockfd, (struct sockaddr *)&server, len) < 0){
            perror("connect()");
            exit(1);
        }
        //DBG(L_GREEN"<Recv>"NONE"%s \n", response.msg);
        //memset(request.msg, 0, sizeof(request.msg));
        //scanf("%s", request.msg); 
       // send(sockfd, (void *)&request, sizeof(request), 0);
        //bzero(&response, sizeof(response));
        //recv(sockfd, response.msg, sizeof(response.msg), 0)
        //DBG(RED"Server Info:"NONE" %s\n", response.msg);
        pthread_t recv_t;
        pthread_create(&recv_t, NULL, do_recv, NULL);
        signal(SIGINT, logout);
        while(1){        
            struct ChatMsg msg;
            bzero(&msg, sizeof(msg));
            msg.type = CHAT_WALL;
            strcpy(msg.name, request.name);
            scanf("%[^\n]s", msg.msg);
            getchar();
            if (strlen(msg.msg)) {
                if (msg.msg[0] == '@') msg.type = CHAT_MSG;
                if (msg.msg[0] == '#') msg.type = CHAT_FUNC;
                send(sockfd, (void *)&msg, sizeof(msg), 0);            
            };
        }

    return 0;
}
