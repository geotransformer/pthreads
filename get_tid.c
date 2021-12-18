# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>

int main(){
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    printf("pid is %d, tid is %lx\n", pid, tid);
    return 0;
}
