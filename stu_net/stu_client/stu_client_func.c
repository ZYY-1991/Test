#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stu_client_func.h"
#include "stu_types.h"

//向服务器发送数据
int client_stu_write(int s_fd, void *buf, size_t size){
    int r;
    if((r = write(s_fd, buf, size)) <= 0){
        printf("\n连接服务器失败\n");
        exit(0);
    }
    return r;
}
//从服务器读取数据
int client_stu_read(int s_fd, void *buf, size_t size){
    int r;
    if((r = read(s_fd, buf, size)) <= 0){
        printf("\n连接服务器失败\n");
        exit(0);
    }
    return r;
}
