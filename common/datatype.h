/*************************************************************************
	> File Name: common/datatype.h
	> Author: 
	> Mail: 
	> Created Time: Thu 09 Jul 2020 10:10:20 AM CST
 ************************************************************************/
//球相关
struct Bpoint{
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
    char name[20];//who is kicking
};
struct Socre {
    int red;
    int blue;
};
//球员相关
struct Point {
    int x;
    int y;
};
#define MAX 300
#define NTHREAD 3
struct User {
    int team;//0 RED 1 BLUE
    int fd;//该玩家对应的连接
    char name[20];
    int online;//1 在线 0 不在线
    int flag;//未响应次数
    struct Point loc;
};
//数据交互相关
//登陆相关
struct LogRequest{
    char name[20];
    int team;
    char msg[512];
};
struct LogResponse{
    int type;//0 OK 1 NO
    char msg[512];
};
//游戏期间交互

#define MAX_MSG 4096
//日常消息交互，如聊天，统一为512
//
#define ACTION_KICK 0x01;
#define ACTION_CARRY 0x02;
#define ACTION_STOP 0x04;

struct Ctl{
    int action;
    int dirx;
    int diry;
    int strength;//踢球力度
};

#define FT_HEART 0x01//心跳
#define FT_WALL 0x02//公告
#define FT_MSG 0x04//聊天
#define FT_ACK 0x08//ack
#define FT_CTL 0x10//控制信息
#define FT_GAME 0x20//游戏场景
#define FT_SOCRE 0x40//比分变化
#define FT_GAMEOVER 0x80//gameover
#define FT_FIN 0x100//离场
struct FootBallMsg{
    int type;
    int size;
    int team;
    char name[20];
    char msg[MAX_MSG];
    struct Ctl ctl;
};

#define CHAT_FIN 0x01//断开
#define CHAT_HEART 0x02
#define CHAT_ACK 0x04
#define CHAT_WALL 0x08
#define CHAT_MSG 0x10
#define CHAT_FUNC 0x20
#define CHAT_SYS 0x40
struct ChatMsg{
    int type;
    char name[20];
    char msg[1024];
};
//B
//球场数据
struct Map{
    int width;
    int heighth;
    struct Point start;
    int gate_width;
    int gate_heighth;
};

