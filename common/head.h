/*************************************************************************
	> File Name: head.h
	> Author: 
	> Mail: 
	> Created Time: Mon 06 Jul 2020 04:35:30 PM CST
 ************************************************************************/

#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/epoll.h>
#include "common.h"
#include "color.h"
#include "datatype.h"
#include "udp_server.h"
#include "udp_client.h"
#include "thread_pool.h"
#include "sub_reacter.h"
#include "udp_epoll.h"
#include "client_recv.h"
