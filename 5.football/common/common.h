/*************************************************************************
	> File Name: common.h
	> Author: jzx
	> Mail: 1790592295@qq.com
	> Created Time: Tue 07 Jul 2020 08:45:24 AM CST
 ************************************************************************/

#ifndef _COMMON_H
#define _COMMON_H
char *get_conf_value(const char *path, const char *key);
int socket_create(int port);
void make_block(int fd);
void make_non_block(int fd);
#ifdef _D
#define DBG(fmt, args...) printf(fmt, ##args)
#else
#define DBG(fmt, args...)
#endif
#endif
