/*************************************************************************
	> File Name: udp_client.c
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 11:56:42 AM CST
 ************************************************************************/

#include "head.h"

int socket_udp(){
    int fd;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket()");
        return -1;
    }
    return fd;
}

