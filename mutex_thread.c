# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <unistd.h>


struct fooObject
{
    int f_ref_count;
    pthread_mutex_t f_lock;
    int f_ref_id;
};

struct fooObject *fooObj_alloc(int id)
{
    struct fooObject *fp;
    if ((fp = malloc(sizeof(struct fooObject))) != NULL){
        fp->f_ref_count = 1;
        fp->f_ref_id = id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0){
            free(fp);
            return(NULL);
        }
    }
    return fp;
}

void *foo_increase_ref_count(void *arg){
    struct fooObject *fp;
    pthread_t tid;
    fp = (struct fooObject *)arg;
    tid = pthread_self();
    printf("From tid is %lx\n, ", tid);

    pthread_mutex_lock(&fp->f_lock);
    fp->f_ref_count++;
    printf("f_ref_count is %d\n", fp->f_ref_count);
    pthread_mutex_unlock(&fp->f_lock);
    return ((void *)0);
}

void *foo_decrease_ref_count(void *arg){
    struct fooObject *fp;
    pthread_t tid;
    fp = (struct fooObject *)arg;
    tid = pthread_self();
    printf("From tid is %lx\n", tid);

    pthread_mutex_lock(&fp->f_lock);
    fp->f_ref_count--;
    printf("f_ref_count is %d\n", fp->f_ref_count);
    if(fp->f_ref_count == 0){
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    }else
    {
        pthread_mutex_unlock(&fp->f_lock);
    }
    return ((void *)0);
}


int main(){
    pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
    int err;
    struct fooObject *fooObj;

    fooObj = fooObj_alloc(1);

    err = pthread_create(&tid1, NULL, foo_increase_ref_count, fooObj);
    if(err != 0) {
        printf("Failed to create thread 1\n");
        return 1;
    }
    err = pthread_create(&tid2, NULL, foo_increase_ref_count, fooObj);
    if(err != 0) {
        printf("Failed to create thread 2\n");
        return 1;
    }
    err = pthread_create(&tid3, NULL, foo_increase_ref_count, fooObj);
    if(err != 0) {
        printf("Failed to create thread 3\n");
        return 1;
    }
    err = pthread_create(&tid4, NULL, foo_decrease_ref_count, fooObj);
    if(err != 0) {
        printf("Failed to create thread 4\n");
        return 1;
    }
    err = pthread_create(&tid5, NULL, foo_decrease_ref_count, fooObj);
    if(err != 0) {
        printf("Failed to create thread 5\n");
        return 1;
    }
    err = pthread_create(&tid6, NULL, foo_decrease_ref_count, fooObj);
    if(err != 0) {
        printf("Failed to create thread 6\n");
        return 1;
    }

    printf("Main thread: create threads successfully\n");
    sleep(2);

    /*
    vagrant@vagrant:/vagrant/pthreads$ ./a.out
        From tid is 7f8f0cc95700
        , f_ref_count is 2
        From tid is 7f8f0b492700
        f_ref_count is 1
        From tid is 7f8f0ac91700
        f_ref_count is 0
        Main thread: create threads successfully
        From tid is 7f8f03fff700
        f_ref_count is -1
        From tid is 7f8f0c494700
        , f_ref_count is 0
        From tid is 7f8f0bc93700
        , f_ref_count is 1
    */

    return 0;
}
