#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include "stu_server_func.h"
#include "stu_link.h"
//获取配置文件信息
int get_server_info(short *p_port){ 
    char buf[128]="";
    int fd = open("server_config.conf", O_RDONLY);
    if(fd == -1){
        printf("获取配置文件信息失败,请检查当前路径和程序路径是否一致\n");
        return -1;
    }
    if(read(fd, buf, 128) == -1){
        perror("read");
        return -1;
    }
    close(fd);
    sscanf(buf, "port %hd", p_port);
    return 0;
}
//创建socket并监听
int create_socket(short port, int num){ 
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(s_fd == -1){
        perror("socket");
        return -1;
    }
    int val = 1;
    if(setsockopt(s_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) == -1){
        perror("setsockopt");
        return -1;
    }
    SA4 serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(s_fd, (SA *)&serv, sizeof(serv)) == -1){
        perror("bind");
        return -1;
    }
    listen(s_fd, num);
    return s_fd;
}
//主要业务
void server_stu_trans(int c_fd, stu_link *p_link){
    int choice, tmp;
    while(1){
        server_stu_read(c_fd, &choice, sizeof(int));
        if(choice == 1 || choice == 3 || choice == 4){
            if(pthread_mutex_trylock(&mutex)){
                tmp = 0;
                server_stu_write(c_fd, &tmp, sizeof(int));
                continue;
            }else{
                tmp = 1;
                server_stu_write(c_fd, &tmp, sizeof(int));
            }
            if(choice == 1){
                server_stu_insert(c_fd, p_link);
            }else if(choice == 3){
                server_stu_delete(c_fd, p_link);
            }else{
                server_stu_change(c_fd, p_link);
            }
            server_write_file(p_link);
            pthread_mutex_unlock(&mutex);
        }else if(choice == 2){
            server_stu_print(c_fd, p_link);
        }else if(choice == 5){
            server_stu_search(c_fd, p_link);
        }else if(choice == 6){
            server_stu_download(c_fd, p_link);
        }
    }
}
