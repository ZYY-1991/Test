#ifndef __STU_SERVER_FUNC_H__
#define __STU_SERVER_FUNC_H__

#include <stdio.h>
#include <netinet/in.h>
#include "stu_types.h"
typedef struct sockaddr_in SA4;
typedef struct sockaddr SA;
typedef struct{
    int c_fd;
    stu_link *p_link;
}pthread_arg;
extern pthread_mutex_t mutex;
extern pthread_mutex_t tmp_mutex;
//获取配置文件信息  参数：用来接受端口的地址  返回值：成功 0 失败 -1
int get_server_info(short *);
//创建socket并监听  参数：端口,监听数量  返回值：成功 描述符 失败 -1
int create_socket(short, int);
//主要业务  参数：描述符,链表地址
void server_stu_trans(int, stu_link *);
//写入文件
void server_write_file(stu_link *);
//读取文件
void server_read_file(stu_link *);
//向客户端发送数据(处理异常情况)
int server_stu_write(int, void *, size_t);
//从客户端读取数据(处理异常情况)
int server_stu_read(int, void *, size_t);
//插入数据
void server_stu_insert(int, stu_link *);
//显示数据
void server_stu_print(int, stu_link *);
//删除数据
void server_stu_delete(int, stu_link *);
//修改数据
void server_stu_change(int, stu_link *);
//下载数据
void server_stu_download(int, stu_link *);
//查找数据
void server_stu_search(int, stu_link *);
//根据id查找
void server_stu_search_by_id(int, stu_link *);
//根据成绩段查找
void server_stu_search_by_score(int, stu_link *);
//根据年龄段查找
void server_stu_search_by_age(int, stu_link *);
//根据名字查找
void server_stu_search_by_name(int, stu_link *);
#endif

