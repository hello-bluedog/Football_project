/*************************************************************************
	> File Name: udp_server.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 11:15:47 AM CST
 ************************************************************************/

#include "head.h"

int socket_create_udp(int port){
    int server_listen = 0;

    server_listen = socket(AF_INET, SOCK_DGRAM, 0);
    if( server_listen < 0){
        return -1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(server_listen, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    make_non_block(server_listen);

    if(bind(server_listen, (struct sockaddr *)&server, sizeof(server)) < 0){
        return -1;
    }
    
    return server_listen;
}

