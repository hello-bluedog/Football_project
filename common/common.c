#include "head.h"
char conf_ans[50] = {0};
    
int socket_create(int port){

    int sockfd;
    struct sockaddr_in server;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){

        return -1;

    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
    if(bind(sockfd, (struct sockaddr*)&server, sizeof(server) )< 0){
        return -1;
    }
    if(listen(sockfd, 10) < 0){
        return -1;
        
    }
    return sockfd;
}

void make_non_block(int fd){
    unsigned long ul = 1;
    ioctl(fd, FIONBIO, &ul);
}

void make_block(int fd){
    unsigned long ul = 0;
    ioctl(fd, FIONBIO, &ul);
}
char *get_conf_value(const char *path, const char *key){
    memset(conf_ans, 0, 50);
    FILE *fp = NULL;
    ssize_t nrd;
    char *sub;
    char *line = NULL;
    ssize_t len;
    if (path == NULL || key == NULL){
        fprintf(stderr, "Error in argument!");
        return NULL;
    }
    if((fp = fopen(path, "r")) == NULL){
        perror("fopen()");
        return NULL;
    }
    while((nrd = getline(&line, &len, fp)) != -1){
        if ((sub = strstr(line, key)) == NULL) continue;
        if (line[strlen(key)] == '='){
            strncpy(conf_ans, sub + strlen(key) + 1,nrd - strlen(key) - 2);
            break;
        }
    }
    
    free(line);
    fclose(fp);
    if(sub == NULL)
        return NULL;
    return conf_ans;
}

