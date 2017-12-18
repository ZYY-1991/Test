#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stu_client_func.h"
#include "stu_types.h"

//向服务器发送数据
int client_stu_write(int s_fd, void *buf, size_t size){
    int r;
    if((r = write(s_fd, buf, size)) <= 0){
        printf("\n连接服务器失败\n");
        exit(0);
    }
    return r;
}
//从服务器读取数据
int client_stu_read(int s_fd, void *buf, size_t size){
    int r;
    if((r = read(s_fd, buf, size)) <= 0){
        printf("\n连接服务器失败\n");
        exit(0);
    }
    return r;
}
//插入数据
void client_stu_insert(int s_fd){
    int id=-1,tmp;
    do{
        printf("\n请输入要添加的学生学号(1-999)(0.返回):");
        scanf("%d", &id);
        scanf("%*[^\n]");
        scanf("%*c");
    }while(id < 0 || id > 999);
    client_stu_write(s_fd, &id, sizeof(int));
    if(id == 0){
        return;
    }
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n学号已存在\n");
        return;
    }
    student stu_tmp={0};
    stu_tmp.shscore = -1;
    do{
        printf("请输入成绩(0-100):");
        scanf("%hd", &stu_tmp.shscore);
        scanf("%*[^\n]");
        scanf("%*c");
    }while(stu_tmp.shscore < 0 || stu_tmp.shscore > 100);
    do{
        printf("请输入年龄(1-100):");
        scanf("%hd", &stu_tmp.age);
        scanf("%*[^\n]");
        scanf("%*c");
    }while(stu_tmp.age < 1 || stu_tmp.age > 100);
    printf("请输入姓名:");
    fgets(stu_tmp.name, NAME_SIZE, stdin);
    if(strlen(stu_tmp.name) == NAME_SIZE-1 && stu_tmp.name[NAME_SIZE-2] != '\n'){
        scanf("%*[^\n]");
        scanf("%*c");
    }else if(stu_tmp.name[strlen(stu_tmp.name)-1] == '\n'){
        stu_tmp.name[strlen(stu_tmp.name)-1] ='\0';
    }
    client_stu_write(s_fd, &stu_tmp, sizeof(student));
}
//显示数据
void client_stu_print(int s_fd){
    int tmp;
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n没有数据\n");
        return;
    }
    int num;
    client_stu_read(s_fd, &num, sizeof(int));
    printf("\n共有%d名学生：\n", num);
    int i;
    node node_tmp;
    /*for(i=0;i<num;i++){
        client_stu_read(s_fd, &node_tmp, sizeof(node));
        printf("学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name);
    }*/
    while(client_stu_read(s_fd, &node_tmp, sizeof(node)) == sizeof(node)){
        printf("学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name); 
    }
}
//删除数据
void client_stu_delete(int s_fd){
    int id=0,tmp;
    printf("\n请输入要删除的学生学号:");
    scanf("%d", &id);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &id, sizeof(int));
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n学号不存在\n");
        return;
    }
    printf("\n删除成功\n");
}
//修改数据
void client_stu_change(int s_fd){
    int id=0,tmp;
    printf("\n请输入要修改的学生学号:");
    scanf("%d", &id);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &id, sizeof(int));
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n学号不存在\n");
        return;
    }
    node node_tmp;
    client_stu_read(s_fd, &node_tmp, sizeof(node));
    printf("\n学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name);
    student stu_tmp={0};
    stu_tmp.shscore = -1;
    do{
        printf("请输入新的成绩(0-100):");
        scanf("%hd", &stu_tmp.shscore);
        scanf("%*[^\n]");
        scanf("%*c");
    }while(stu_tmp.shscore < 0 || stu_tmp.shscore > 100);
    do{
        printf("请输入新的年龄(1-100):");
        scanf("%hd", &stu_tmp.age);
        scanf("%*[^\n]");
        scanf("%*c");
    }while(stu_tmp.age < 1 || stu_tmp.age > 100);
    printf("请输入新的姓名:");
    fgets(stu_tmp.name, NAME_SIZE, stdin);
    if(strlen(stu_tmp.name) == NAME_SIZE-1 && stu_tmp.name[NAME_SIZE-2] != '\n'){
        scanf("%*[^\n]");
        scanf("%*c");
    }else if(stu_tmp.name[strlen(stu_tmp.name)-1] == '\n'){
        stu_tmp.name[strlen(stu_tmp.name)-1] ='\0';
    }
    client_stu_write(s_fd, &stu_tmp, sizeof(student));
    printf("\n修改成功\n");
}
//下载数据
void client_stu_download(int s_fd){
    int tmp;
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n没有数据\n");
        return;
    }
    int fd = open("stu.txt", O_WRONLY|O_CREAT|O_TRUNC, 0664);
    int i=0;
    char buf[128];
    node node_tmp;
    while(client_stu_read(s_fd, &node_tmp, sizeof(node)) == sizeof(node)){
        sprintf(buf, "学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name);
        write(fd, buf, strlen(buf));
    }
    close(fd);
    printf("\n下载成功:stu.txt\n");
}
//查找数据
void client_stu_search(int s_fd){
    int tmp;
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n没有数据\n");
        return;
    }
    while(1){
        int choice = -1;
        printf("\n1.按学号查找\n");
        printf("2.按成绩段查找\n");
        printf("3.按年龄段查找\n");
        printf("4.按名字查找\n");
        printf("0.返回\n");
        printf("请输入查找的方式:");
        scanf("%d", &choice);
        scanf("%*[^\n]");
        scanf("%*c");
        if(choice == 1){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_search_by_id(s_fd);
        }else if(choice == 2){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_search_by_score(s_fd);
        }else if(choice == 3){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_search_by_age(s_fd);
        }else if(choice == 4){
            client_stu_write(s_fd, &choice, sizeof(int));
            client_stu_search_by_name(s_fd);
        }else if(choice == 0){
            client_stu_write(s_fd, &choice, sizeof(int));
            return;
        }
    }
    
}
//根据id查找
void client_stu_search_by_id(int s_fd){
    int id=0,tmp;
    printf("\n请输入要查找的学生学号:");
    scanf("%d", &id);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &id, sizeof(int));
    client_stu_read(s_fd, &tmp, sizeof(int));
    if(tmp == 0){
        printf("\n学号不存在\n");
        return;
    }
    node node_tmp;
    client_stu_read(s_fd, &node_tmp, sizeof(node));
    printf("\n学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name);
}
//根据成绩段查找
void client_stu_search_by_score(int s_fd){
    int score_l = 101,score_h = -1;
    printf("请输入要查找的最小分数:");
    scanf("%d", &score_l);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &score_l, sizeof(int));
    printf("请输入要查找的最大分数:");
    scanf("%d", &score_h);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &score_h, sizeof(int));
    printf("\n");
    node node_tmp;
    while(client_stu_read(s_fd, &node_tmp, sizeof(node)) == sizeof(node)){
        printf("学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name); 
    }
    int tmp=0;
    client_stu_write(s_fd, &tmp, sizeof(int));
    int num;
    client_stu_read(s_fd, &num, sizeof(int));
    printf("共有%d名学生符合要求\n", num);
}
//根据年龄查找
void client_stu_search_by_age(int s_fd){
    int age_l = 101,age_h = 0;
    printf("请输入要查找的最小年龄:");
    scanf("%d", &age_l);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &age_l, sizeof(int));
    printf("请输入要查找的最大年龄:");
    scanf("%d", &age_h);
    scanf("%*[^\n]");
    scanf("%*c");
    client_stu_write(s_fd, &age_h, sizeof(int));
    printf("\n");
    node node_tmp;
    while(client_stu_read(s_fd, &node_tmp, sizeof(node)) == sizeof(node)){
        printf("学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name); 
    }
    int tmp=0;
    client_stu_write(s_fd, &tmp, sizeof(int));
    int num;
    client_stu_read(s_fd, &num, sizeof(int));
    printf("共有%d名学生符合要求\n", num);
}
//根据名字查找
void client_stu_search_by_name(int s_fd){
    char name[NAME_SIZE]="";
    printf("请输入要查找的名字:");
    fgets(name, NAME_SIZE, stdin);
    if(strlen(name) == NAME_SIZE-1 && name[NAME_SIZE-2] != '\n'){
        scanf("%*[^\n]");
        scanf("%*c");
    }else if(name[strlen(name)-1] == '\n'){
        name[strlen(name)-1] = '\0';
    }
    client_stu_write(s_fd, name, NAME_SIZE);
    printf("\n");
    node node_tmp;
    while(client_stu_read(s_fd, &node_tmp, sizeof(node)) == sizeof(node)){
        printf("学号:%3d  成绩:%3hd  年龄:%3hd  姓名:%s\n", node_tmp.id, node_tmp.stu.shscore, node_tmp.stu.age, node_tmp.stu.name); 
    }
    int tmp=0;
    client_stu_write(s_fd, &tmp, sizeof(int));
    int num;
    client_stu_read(s_fd, &num, sizeof(int));
    printf("共有%d名学生符合要求\n", num);
}

