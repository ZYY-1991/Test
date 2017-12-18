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
//插入数据
void server_stu_insert(int c_fd, stu_link *p_link){
    int id=-1, tmp;
    server_stu_read(c_fd, &id, sizeof(int));
    if(id == 0){
        return;
    }
    if(link_find(p_link, id)){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    student stu_tmp;
    server_stu_read(c_fd, &stu_tmp, sizeof(student));
    pthread_mutex_lock(&tmp_mutex);
    link_insert(p_link, id, &stu_tmp);
    pthread_mutex_unlock(&tmp_mutex);
}
//显示数据
void server_stu_print(int c_fd, stu_link *p_link){
    int tmp;
    if(link_empty(p_link)){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    int num = link_size(p_link);
    server_stu_write(c_fd, &num, sizeof(int));
    pthread_mutex_lock(&tmp_mutex);
    link_begin(p_link);
    node *p_tmp;
    /*int i;
    for(i=0;i<num;i++){
        p_tmp = link_next(p_link);
        server_stu_write(c_fd, p_tmp, sizeof(node));
    }*/
    while(p_tmp = link_next(p_link)){
        server_stu_write(c_fd, p_tmp, sizeof(node));
    }
    pthread_mutex_unlock(&tmp_mutex);
    tmp = 0;
    server_stu_write(c_fd, &tmp, sizeof(int));
}
//删除数据
void server_stu_delete(int c_fd, stu_link *p_link){
    int id, tmp;
    server_stu_read(c_fd, &id, sizeof(int));
    if(!link_find(p_link, id)){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    pthread_mutex_lock(&tmp_mutex);
    link_delete(p_link, id);
    pthread_mutex_unlock(&tmp_mutex);
}
//修改数据
void server_stu_change(int c_fd, stu_link *p_link){
    int id, tmp;
    server_stu_read(c_fd, &id, sizeof(int));
    node *p_tmp = link_find(p_link, id);
    if(!p_tmp){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    server_stu_write(c_fd, p_tmp, sizeof(node));
    student stu_tmp;
    server_stu_read(c_fd, &stu_tmp, sizeof(student));
    pthread_mutex_lock(&tmp_mutex);
    link_delete(p_link, id);
    link_insert(p_link, id, &stu_tmp);
    pthread_mutex_unlock(&tmp_mutex);
}
//下载数据
void server_stu_download(int c_fd, stu_link *p_link){
    int tmp;
    if(link_empty(p_link)){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    pthread_mutex_lock(&tmp_mutex);
    link_begin(p_link);
    int i;
    node *p_tmp;
    while(p_tmp = link_next(p_link)){
        server_stu_write(c_fd, p_tmp, sizeof(node));
    }
    pthread_mutex_unlock(&tmp_mutex);
    tmp = 0;
    server_stu_write(c_fd, &tmp, sizeof(int));
}
//查找数据
void server_stu_search(int c_fd, stu_link *p_link){
    int tmp;
    if(link_empty(p_link)){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    int choice;
    while(1){
        server_stu_read(c_fd, &choice, sizeof(int));
        if(choice == 1){
            server_stu_search_by_id(c_fd, p_link);
        }else if(choice == 2){
            server_stu_search_by_score(c_fd, p_link);
        }else if(choice == 3){
            server_stu_search_by_age(c_fd, p_link);
        }else if(choice == 4){
            server_stu_search_by_name(c_fd, p_link);
        }else if(choice == 0){
            return;
        }
    }
}
//根据id查找
void server_stu_search_by_id(int c_fd, stu_link *p_link){
    int id=0, tmp;
    server_stu_read(c_fd, &id, sizeof(int));
    node *p_tmp = link_find(p_link, id);
    if(!p_tmp){
        tmp = 0;
        server_stu_write(c_fd, &tmp, sizeof(int));
        return;
    }else{
        tmp = 1;
        server_stu_write(c_fd, &tmp, sizeof(int));
    }
    server_stu_write(c_fd, p_tmp, sizeof(node));
}
//根据成绩段查找
void server_stu_search_by_score(int c_fd, stu_link *p_link){
    int score_l,score_h;
    server_stu_read(c_fd, &score_l, sizeof(int));
    server_stu_read(c_fd, &score_h, sizeof(int));
    pthread_mutex_lock(&tmp_mutex);
    link_begin(p_link);
    node *p_tmp;
    int num=0;
    while(p_tmp = link_next(p_link)){
        if(p_tmp->stu.shscore >= score_l && p_tmp->stu.shscore <= score_h){
            server_stu_write(c_fd, p_tmp, sizeof(node));
            num++;
        }
    }
    pthread_mutex_unlock(&tmp_mutex);
    int tmp=0;
    server_stu_write(c_fd, &tmp, sizeof(int));
    server_stu_read(c_fd, &tmp, sizeof(int));
    server_stu_write(c_fd, &num, sizeof(int));
}
//根据年龄段查找
void server_stu_search_by_age(int c_fd, stu_link *p_link){
    int age_l,age_h;
    server_stu_read(c_fd, &age_l, sizeof(int));
    server_stu_read(c_fd, &age_h, sizeof(int));
    pthread_mutex_lock(&tmp_mutex);
    link_begin(p_link);
    node *p_tmp;
    int num=0;
    while(p_tmp = link_next(p_link)){
        if(p_tmp->stu.age >= age_l && p_tmp->stu.age <= age_h){
            server_stu_write(c_fd, p_tmp, sizeof(node));
            num++;
        }
    }
    pthread_mutex_unlock(&tmp_mutex);
    int tmp=0;
    server_stu_write(c_fd, &tmp, sizeof(int));
    server_stu_read(c_fd, &tmp, sizeof(int));
    server_stu_write(c_fd, &num, sizeof(int));
}
//根据名字查找
void server_stu_search_by_name(int c_fd, stu_link *p_link){
    char name[NAME_SIZE];
    server_stu_read(c_fd, name, NAME_SIZE);
    pthread_mutex_lock(&tmp_mutex);
    link_begin(p_link);
    node *p_tmp;
    int num=0;
    while(p_tmp = link_next(p_link)){
        if(strcmp(p_tmp->stu.name, name) == 0){
            server_stu_write(c_fd, p_tmp, sizeof(node));
            num++;
        }
    }
    pthread_mutex_unlock(&tmp_mutex);
    int tmp=0;
    server_stu_write(c_fd, &tmp, sizeof(int));
    server_stu_read(c_fd, &tmp, sizeof(int));
    server_stu_write(c_fd, &num, sizeof(int));
}

