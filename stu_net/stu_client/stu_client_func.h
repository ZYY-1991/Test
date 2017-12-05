#ifndef __STU_CLIENT_FUNC_H__
#define __STU_CLIENT_FUNC_H__

#include <stdio.h>
#include <netinet/in.h>
typedef struct sockaddr_in SA4;
typedef struct sockaddr SA;
//获取配置文件信息  参数：用来接受IP字符串的地址，用来接受端口的地址  返回值：成功 0 失败 -1
int get_client_info(char *, short *);
//连接服务器  参数：IP，端口  返回值：成功 描述符 失败 -1
int connect_socket(char *, short);
//主要业务  参数：描述符
void client_stu_trans(int);
//向服务器发送数据(处理异常情况)
int client_stu_write(int, void *, size_t);
//从服务器读取数据(处理异常情况)
int client_stu_read(int, void *, size_t);
//插入数据
void client_stu_insert(int);
//显示数据
void client_stu_print(int);
//删除数据
void client_stu_delete(int);
//修改数据
void client_stu_change(int);
//下载数据
void client_stu_download(int);
//查找数据
void client_stu_search(int);
//根据id查找
void client_stu_search_by_id(int);
//根据成绩段查找
void client_stu_search_by_score(int);
//根据年龄段查找
void client_stu_search_by_age(int);
//根据名字查找
void client_stu_search_by_name(int);
#endif

