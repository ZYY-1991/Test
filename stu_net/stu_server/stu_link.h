#ifndef __STU_LINK_H__
#define __STU_LINK_H__

#include "stu_types.h"
//初始化
void link_init(stu_link *);
//清理
void link_deinit(stu_link *);
//判断空  返回值：空 1 非空 0
int link_empty(const stu_link *);
//统计数量  返回值：节点数量
int link_size(const stu_link *);
//根据id查找  返回值：成功 找到的节点地址 失败 NULL
node *link_find(const stu_link *, int);
//根据id顺序插入  返回值：成功 1 失败 0
int link_insert(stu_link *, int, student *);
//根据id删除  返回值：成功 1 失败 0
int link_delete(stu_link *, int);
//开始遍历
void link_begin(stu_link *);
//遍历下一个节点  返回值：成功 下一个节点地址 失败 NULL
node *link_next(stu_link *);

#endif

