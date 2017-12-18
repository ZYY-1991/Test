#include <stdlib.h>
#include "stu_link.h"
//初始化
void link_init(stu_link *p_link){
    p_link->head.p_next = &(p_link->tail);
    p_link->tail.p_next = NULL;
    p_link->p_cur = NULL;
}
//清理
void link_deinit(stu_link *p_link){
    node *p_tmp = NULL;
    p_link->p_cur = NULL;
    while(!link_empty(p_link)){
        p_tmp = p_link->head.p_next;
        p_link->head.p_next = p_tmp->p_next;
        free(p_tmp);
        p_tmp = NULL;
    }
}
//判断空
int link_empty(const stu_link *p_link){
    return p_link->head.p_next == &(p_link->tail);
}
//统计数量
int link_size(const stu_link *p_link){
    const node *p_node = NULL;
    int num = 0;
    for(p_node = &(p_link->head);p_node != &(p_link->tail);p_node = p_node->p_next){
        const node *p_first = p_node;
        const node *p_mid = p_node->p_next;
        if(p_mid == &(p_link->tail)){
            break;
        }
        num++;
    }
    return num;
}
//根据id查找
node *link_find(const stu_link *p_link, int id){
    const node *p_node = NULL;
    for(p_node = &(p_link->head);p_node != &(p_link->tail);p_node = p_node->p_next){
        const node *p_first = p_node;
        const node *p_mid = p_node->p_next;
        if(p_mid != &(p_link->tail) && p_mid->id == id){
            return (node *)p_mid;
        }
    }
    return NULL;
}
//根据id顺序插入
int link_insert(stu_link *p_link, int id, student *p_stu){
    node *p_node = NULL;
    p_link->p_cur = NULL;
    for(p_node = &(p_link->head);p_node != &(p_link->tail);p_node = p_node->p_next){
        node *p_first = p_node;
        node *p_mid = p_node->p_next;
        if(p_mid == &(p_link->tail) || p_mid->id > id){
            node *p_tmp = (node *)malloc(sizeof(node));
            p_tmp->id = id;
            p_tmp->stu = *p_stu;
            p_tmp->p_next = p_mid;
            p_first->p_next = p_tmp;
            return 1;
        }
    }
    return 0;
}
//根据id删除
int link_delete(stu_link *p_link, int id){
    node *p_node = NULL;
    p_link->p_cur = NULL;
    for(p_node = &(p_link->head);p_node != &(p_link->tail);p_node = p_node->p_next){
        node *p_first = p_node;
        node *p_mid = p_node->p_next;
        if(p_mid != &(p_link->tail) && p_mid->id == id){
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
    p_link->p_cur = &(p_link->head);
}
//遍历下一个节点
node *link_next(stu_link *p_link){
    if(!p_link->p_cur){
        return NULL;
    }
    p_link->p_cur = p_link->p_cur->p_next;
    if(p_link->p_cur == &(p_link->tail)){
        p_link->p_cur = NULL;
        return NULL;
    }else{
        return p_link->p_cur;
    }
}

