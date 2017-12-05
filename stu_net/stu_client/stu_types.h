#ifndef __STU_TYPES_H__
#define __STU_TYPES_H__

#define NAME_SIZE 30
typedef struct{
    short shscore;
    short age;
    char name[NAME_SIZE];
}student;

typedef struct node{
    int id;
    student stu;
    struct node *p_next;
}node;

typedef struct{
    node head,tail;
    node *p_cur;
}stu_link;

#endif

