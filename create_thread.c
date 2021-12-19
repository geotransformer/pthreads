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

void print_id(char *name){
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    printf("From %s: pid is %d, tid is %lx\n", name, pid, tid);
}

void *thread_func(void *name){
    print_id(name);
    return ((void *)0);
}

int main(){
    pthread_t tid;
    int err;
    /*
     * Race between main thread and new thread
     * 1) Sleep in the main thread. Main thread might exit and terminate the entire process
     * Then, the new thread might get change to execute
     *
     * 2) New thread needs to call pthread_self to get the tio. Should not get it from sahred memory
     *  or receive tid from the argument of thread-start routine, i.e., thread_func(). 
     *
     *  New thread can run before the pthread_create returns and populate tid, i.e., &tid
     */

    err = pthread_create(&tid, NULL, thread_func, "new thread");
    if(err != 0) {
        printf("Failed to create thread\n");
        return 1;
    }
    print_id("main thread:");
    sleep(2);

    return 0;
}
