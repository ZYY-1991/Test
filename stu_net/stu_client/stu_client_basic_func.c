#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stu_client_func.h"
//获取配置文件信息
int get_client_info(char *ip, short *p_port){
    char buf[128]="";
    int fd = open("client_config.conf", O_RDONLY);
    if(fd == -1){
        perror("open");
        printf("获取配置文件信息失败,请检查当前路径和程序路径是否一致\n");
        return -1;
    }
    if(read(fd, buf, 128) == -1){
        perror("read");
        return -1;
    }
    close(fd);
    sscanf(buf, "server_ip %s\nport %hd", ip, p_port);
    return 0;
}
//连接服务器
int connect_socket(char *ip, short port){
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(s_fd == -1){
        perror("socket");
        return -1;
    }
    SA4 serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serv.sin_addr);
    if(connect(s_fd, (SA *)&serv, sizeof(serv)) == -1){
        perror("connect");
        printf("连接服务器失败,请检查服务器状态和配置文件是否正确\n");
        return -1;
    }
    return s_fd;
}
//主要业务
void client_stu_trans(int s_fd){
    while(1){
        int choice = -1;
        int tmp;
        printf("\n************************\n");
        printf("1.插入数据\n");
        printf("2.显示数据\n");
        printf("3.删除数据\n");
        printf("4.修改数据\n");
        printf("5.查找数据\n");
        printf("6.下载数据\n");
        printf("0.退出\n");
        printf("请输入要执行的操作:");
        scanf("%d", &choice);
        scanf("%*[^\n]");
        scanf("%*c");
        if(choice == 1 || choice == 3 || choice == 4){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_read(s_fd, &tmp, sizeof(int));
            if(tmp == 0){
                printf("\n当前有人正在进行操作，请稍后再试\n");
                continue;
            }
            if(choice == 1){
                client_stu_insert(s_fd);
            }else if(choice == 3){
                client_stu_delete(s_fd);
            }else if(choice == 4){
                client_stu_change(s_fd);
            }
        }else if(choice == 2){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_print(s_fd);
        }else if(choice == 5){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_search(s_fd);
        }else if(choice == 6){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_download(s_fd);
        }else if(choice == 0){
            break;
        }
    }
}
