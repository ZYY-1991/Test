#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "stu_server_func.h"
#include "stu_link.h"

//写入文件
void server_write_file(stu_link *p_link){
    int fd = open("stu.bin", O_WRONLY|O_CREAT|O_TRUNC, 0664);
    if(fd == -1){
        return;
    }
    node *p_tmp = NULL;
    pthread_mutex_lock(&tmp_mutex);
    link_begin(p_link);
    while(p_tmp = link_next(p_link)){
        write(fd, p_tmp, sizeof(node));
    }
    pthread_mutex_unlock(&tmp_mutex);
    close(fd);
}
//读取文件
void server_read_file(stu_link *p_link){
    int fd = open("stu.bin", O_RDONLY);
    if(fd == -1){
        return;
    }
    node tmp;
    while(read(fd, &tmp, sizeof(node))){
        link_insert(p_link, tmp.id, &tmp.stu);
    }
    close(fd);
}
//向客户端发送数据
int server_stu_write(int c_fd, void *buf, size_t size){
    int r;
    if((r = write(c_fd, buf, size)) <= 0){
        close(c_fd);
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&tmp_mutex);
        pthread_exit(0);
    }
    return r;
}
//从客户端读取数据
int server_stu_read(int c_fd, void *buf, size_t size){
    int r;
    if((r = read(c_fd, buf, size)) <= 0){
        close(c_fd);
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&tmp_mutex);
        pthread_exit(0);
    }
    return r;
}
