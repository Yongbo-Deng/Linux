#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Use condition to sync between processes.*/

static char g_buf[1000];
static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t g_tConVar = PTHREAD_COND_INITIALIZER;


static void *my_thread_func(void *data)
{
    while (1) {
        // sleep(1);

        /* Wait for notification. */
        pthread_mutex_lock(&g_tMutex);
        pthread_cond_wait(&g_tConVar, &g_tMutex);

        /* Print the recieved message. */
        printf("recv: %s\n", g_buf);
        pthread_mutex_unlock(&g_tMutex);
    }
}


int main(int argc, char **argv)
{
    pthread_t tid;
    int ret;
    char buf[1000];


    /* 1.Create "Recieve thread"*/
    ret = pthread_create(&tid, NULL, my_thread_func, NULL);
    if (ret)
    {
        printf("pthread_create err!\n");
        return -1;
    }

    /* 2. Main process read standard input and send it to "Recieve thread"*/
    while (1) {
        /* Read input into the local buffer to Reduced Mutex Holding Time: */
        fgets(buf, 1000, stdin);

        /* Acquire mutex to safely copy data to the global buffer */
        pthread_mutex_lock(&g_tMutex);
        memcpy(g_buf, buf, 1000);
        pthread_cond_signal(&g_tConVar);    /* Notify the recieve process. */
        pthread_mutex_unlock(&g_tMutex);
    }
    return 0;

}



