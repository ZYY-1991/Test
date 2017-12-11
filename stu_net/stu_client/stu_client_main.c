#include <unistd.h>
#include "stu_client_func.h"
//main
int main(void){
    char ip[64]="";
    short port=0;
    if(get_client_info(ip, &port) == -1){
        return -1;
    }
    int s_fd = connect_socket(ip, port);
    if(s_fd == -1){
        return -1;
    }
    client_stu_trans(s_fd);
    close(s_fd);
    return 0;
}

