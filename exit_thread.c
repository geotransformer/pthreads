# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

/*
 * If any thread in a process calls exit, _Exit, or _exit, then entire process terminates.
 *
 * Thread can exit in 3 ways, but wont terminate the process
 * 1) simply return from start routine 
 * 2) Thread is cancelled
 * 3) Thread calls pthread_exit
 */

void *thread_func(void *arg){
    if (strcmp("1", (char *)arg) == 0){
        return ((void *)1); 
    }
    if (strcmp("2", (char *)arg) == 0){
        pthread_exit((void *)2);
    }
    printf("New thread calls exit() \n");
    exit(3);
}

int main(){
    pthread_t tid;
    int err;
    void * tret;
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
    err = pthread_create(&tid, NULL, thread_func, "2");
    if(err != 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    printf("Before thread join %ld\n", (long)tret);

    err = pthread_join(tid, &tret);
    if(err != 0) {
        printf("Failed to join thread\n");
        return 1;
    }

    printf("Thread exited with code %ld\n", (long)tret);
    /*
     * vagrant@vagrant:/vagrant/pthreads$ ./a.out
     * main thread sleep
     * New thread is exiting with 3
     * vagrant@vagrant:/vagrant/pthreads$ echo $?
     * 3
     */
    printf("main thread sleep\n");
    sleep(2);
    printf("main thread wake up\n");

    return 0;
}
