#include "stu_types.h"
#include <stdlib.h>
//初始化
void link_init(stu_link *p_link){
    p_link->head.p_next = &(p_link->tail);
    p_link->tail.p_next = NULL;
    p_link->p_cur = NULL;
}
//清理
void link_deinit(stu_link *p_link){
    p_link->p_cur = NULL;
    while(p_link->head.p_next != &(p_link->tail)){
        node *p_first = &(p_link->head);
        node *p_mid = p_first->p_next;
        node *p_last= p_mid->p_next;
        p_first->p_next= p_last;
        free(p_mid);
        p_mid = NULL;
    }
}

//判断空  返回值：空 1 非空 0
int link_empty(const stu_link *p_link){
    if(p_link->head.p_next != &(p_link->tail)){
        return 0;
    }
    return 1;
}
//统计数量  返回值：节点数量
int link_size(const stu_link *p_link){
    int cnt = 0;
    const node *p_node = NULL;
    for(p_node = &(p_link->head);p_node != &(p_link->tail);p_node = p_node->p_next){
        const node *p_first = p_node;
        const node *p_mid = p_first->p_next;
        const node *p_last = p_mid->p_next;
        if(p_mid == &(p_link->tail)){
            break;
        }
        cnt++;
    }
    return cnt;
}
//根据id查找  返回值：成功 找到的节点地址 失败 NULL
node *link_find(const stu_link *p_link, int num){
    const node *p_node = NULL;
    for(p_node = &(p_link->head);p_node != &(p_link->tail);p_node = p_node->p_next){
       
        const node *p_first = p_node;
        const node *p_mid = p_first->p_next;
        const node *p_last = p_mid->p_next;
        if(p_mid->id==num){
            return (node *)p_mid;
        }
    }
    return NULL;
}
//根据id顺序插入  返回值：成功 1 失败 0
int link_insert(stu_link *p_link, int num, student *stu){
    node *p_node = NULL;
    node *p_tmp = NULL;
    p_link->p_cur = NULL;
    p_node=(node *)malloc(sizeof(node));
    if(!p_node){
        return 0;
    }
    p_node->id = num;
    p_node->stu = *stu;
    p_node->p_next = NULL;
    for(p_tmp = &(p_link->head);p_tmp != &(p_link->tail);p_tmp = p_tmp->p_next){
        node *p_first = p_tmp;
        node *p_mid = p_first->p_next;
        node *p_last = p_mid->p_next;
        if(p_mid != &(p_link->tail)||p_mid->id > num){
            p_node->p_next = p_mid;
            p_first->p_next = p_node;
            return 1;
        }
    }
    return 0;
}

//根据id删除  返回值：成功 1 失败 0
int link_delete(stu_link *p_link, int num){
    p_link->p_cur = NULL;
    node *p_tmp = NULL;
    for(p_tmp = &(p_link->head);p_tmp != &(p_link->tail);p_tmp = p_tmp->p_next){
        node *p_first = p_tmp;
        node *p_mid = p_first->p_next;
        node *p_last = p_mid->p_next;
        if(p_mid != &(p_link->tail) && p_mid->id==num){
            p_first->p_next = p_mid->p_next;
            free(p_mid);
            p_mid = NULL;
            return 1;
        }
    }
    return 0;
}
//开始遍历
void link_begin(stu_link *p_link){
    p_link->p_cur= &(p_link->head);
}
//遍历下一个节点  返回值：成功 下一个节点地址 失败 NULL
node *link_next(stu_link *p_link){
    if(!(p_link->p_cur)){
        return NULL;
    }
    p_link->p_cur = p_link->p_cur->p_next;
    if(p_link->p_cur== &(p_link->tail)){
        p_link->p_cur = NULL;
        return p_link->p_cur;
    }
    else{
        return p_link->p_cur;
    }
    
}


