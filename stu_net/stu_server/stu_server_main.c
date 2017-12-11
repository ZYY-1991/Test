#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "stu_server_func.h"
#include "stu_link.h"
pthread_mutex_t mutex;
pthread_mutex_t tmp_mutex;
//doit
void *doit(void *arg){
    server_stu_trans(((pthread_arg *)arg)->c_fd, ((pthread_arg *)arg)->p_link);
    return NULL;
}
//main
int main(void){
    short port=0;
    if(get_server_info(&port) == -1){
        return -1;
    }
    int s_fd = create_socket(port, 100);
    if(s_fd == -1){
        return -1;
    }
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setkind_np(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&mutex, &mutex_attr);
    pthread_mutex_init(&tmp_mutex, &mutex_attr);
    stu_link lnk;
    link_init(&lnk);
    server_read_file(&lnk);
    while(1){
        int c_fd = accept(s_fd, NULL, NULL);
        if(c_fd == -1){
            perror("accept");
            return -1;
        }
        pthread_t tid;
        pthread_arg arg;
        arg.c_fd = c_fd;
        arg.p_link = &lnk;
        pthread_create(&tid, NULL, doit, &arg);
    }
    pthread_mutex_destroy(&tmp_mutex);
    pthread_mutex_destroy(&mutex);
    link_deinit(&lnk);
    close(s_fd);
    return 0;
}

