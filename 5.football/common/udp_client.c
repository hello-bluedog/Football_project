/*************************************************************************
	> File Name: udp_client.c
	> Author: jzx
	> Mail: 1790592295@qq.com
	> Created Time: Thu 09 Jul 2020 11:56:37 AM CST
 ************************************************************************/

#include "head.h"

int socket_udp(){
    int fd;

    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        return -1;
    }
    return fd;
}
