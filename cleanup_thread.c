# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

/*
 *  #include <pthread.h>
 *  int pthread_create(pthread_t *restrict thread,
 *                     const pthread_attr_t *restrict attr,
 *                     void *(*start_routine)(void *),
 *                     void *restrict arg);
 *      Compile and link with -pthread.
 */

void cleanup(void *arg){
    printf("Clean up: %s\n", (char *)arg);
}

void *thread_func1(void *arg){
    printf("**T1 start\n");
    pthread_cleanup_push(cleanup, "T1 **first cleanup handler");
    pthread_cleanup_push(cleanup, "T1 second** cleanup handler");
    printf("**T1 push complete\n");
    if (arg){
        return ((void *)1);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return ((void *)1);
}

void *thread_func2(void *arg){
    printf("T2** start\n");
    pthread_cleanup_push(cleanup, "T2 **first cleanup handler");
    pthread_cleanup_push(cleanup, "T2 second** cleanup handler");
    printf("T2** push complete\n");
    if (arg){
        pthread_exit((void *)2);
    }
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

int main(){
    pthread_t tid1, tid2;
    int err;
    void *tret;

    err = pthread_create(&tid1, NULL, thread_func1, "T2");
    if(err != 0) {
        printf("Failed to create thread 1\n");
        return 1;
    }
    err = pthread_create(&tid2, NULL, thread_func2, "T2");
    if(err != 0) {
        printf("Failed to create thread 2\n");
        return 1;
    }
    pthread_join(tid1, &tret);
    printf("T1 exit code %ld\n", (long)tret);
    
    pthread_join(tid2, &tret);
    printf("T2 exit code %ld\n", (long)tret);

    printf("Main thread: create threads successfully\n");
    sleep(2);

    return 0;
}
