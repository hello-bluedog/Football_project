/*************************************************************************
	> File Name: datatype.h
	> Author: jzx
	> Mail: 1790592295@qq.com
	> Created Time: Thu 09 Jul 2020 10:11:21 AM CST
 ************************************************************************/

#ifndef _DATATYPE_H
#define _DATATYPE_H

#define MAX 300 //用户数的二分之一
#define NTHREAD 3
#define CHAT_FIN 0x01 //断开
#define CHAT_HEART 0x02
#define CHAT_ACK 0x04
#define CHAT_WALL 0x08 //公聊
#define CHAT_MSG 0x10 //私聊
#define CHAT_FUNC 0x20//功能
#define CHAT_SYS 0x40
struct ChatMsg {
    int type;
    char name[20];
    char msg[1024];
};

/*struct Bpoint{
    double x;
    double y;
};

struct Speed{
    double x;
    double y;
};

struct Aspeed{
    double x;
    double y;
};

struct BallStatus{
    struct Speed v;
    struct Aspeed a;
    int who;//which team
    char name[20];
};
*以上关于球*/

struct Point{
    int x;
    int y;
};

struct User{
    int team;//0 red,1 bule
    int fd;//该玩家对应的连接
    char name[20];
    int online;//1为在线，0为不在线
    int flag;//为响应次数
    struct Point loc;
};
/*以上为球员*/

/*以下登录相关的*/
struct LogRequest{
    char name[20];
    int team;
    char msg[512];
};

struct LogResponse{
    int type;//0 ok,1 no
    char msg[512];
};
/*
*以下游戏期间的交互
#define MAX_MSG 4096
//日常的信息交互，如聊天信息，统一为512长度
*
#define ACTION_KICK 0x01
#define ACTION_CARRY 0x02
#define ACTION_STOP 0x04

struct Ctl{
    int action;//自己定义
    int dirx;
    int diry;
    int strength;//踢球力度
};

#define FT_HEART 0x01//心跳
#define FT_WALL 0x02//公告
#define FT_MSG 0x04//聊天
#define FT_ACK 0x08//ack
#define FT_CTL 0x10//控制信息
#define FT_GAME 0x20//游戏场景数据
#define FT_SCORE 0x40//比分变化
#define FT_GAMEOVER 0x80//gameover
#define FT_FIN 0x100//离场

struct FootBallMsg{
    int type;//使用时，type & FT_HEART, 与运算 
    int size;
    int team;
    char name[20];
    char msg[MAX_MSG];
    struct Ctl ctl;
};

*以下为球场*/
struct Map{
    int width;
    int height;
    struct Point start;
    int gate_width;
    int gate_height;
};

struct Score{
    int red;
    int bule;
};

#endif
