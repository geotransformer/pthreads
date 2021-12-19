# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

/*
 * main thread is created with the creation of process
 * main thread finishes and it will terminate hte process
 * Call thread_exit in main thread will keep the process running
 *
 */

struct student {
    int age;
    char name[20];
};

void *thread_func(void *stu){
    printf("Student age is %d, name is %s\n", ((struct student *)stu)->age, ((struct student *)stu)->name);
    return ((void *)0);
}

int main(int argc, char *argv[]){
    pthread_t tid;
    int err;
    int i;
    struct student a;
    a.age = 20;
    memcpy(a.name, "aaaaaaa",20);
    
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
    err = pthread_create(&tid, NULL, thread_func, (void *)(&a));
    if(err != 0) {
        printf("Failed to create thread\n");
        return 1;
    }

    /*
     * vagrant@vagrant:/vagrant/pthreads$ ./a.out 1 2 3 4 5 6
     * 0th main thread args is ./a.out
     * Student age is 20, name is aaaaaaa
     * 1th main thread args is 1
     * 2th main thread args is 2
     * 3th main thread args is 3
     * 4th main thread args is 4
     * 5th main thread args is 5
     * 6th main thread args is 6
     */
    for (i = 0; i<argc; i++){
        printf("%dth main thread args is %s\n", i, argv[i]);
    }

    
    pthread_exit((void *)88);
}
