# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>


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
    
    err = pthread_create(&tid, NULL, thread_func, "new thread");
    if(err != 0) {
    	printf("Failed to create thread\n");
	return 1;
    }
    print_id("main thread:");
    sleep(2);

    return 0;
}
