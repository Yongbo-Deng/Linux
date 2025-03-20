#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

/* Use mutex to access to resource exclusively. */

static char g_buf[1000];
static sem_t g_sem;
static pthread_mutex_t g_tMutex = PTHREAD_MUTEX_INITIALIZER;


static void *my_thread_func(void *data)
{
    while (1) {
        // sleep(1);
        /* Wait for notification and print. */
        sem_wait(&g_sem);
        pthread_mutex_lock(&g_tMutex);
        printf("recv: %s\n", g_buf);
        pthread_mutex_unlock(&g_tMutex);
    }
}


int main(int argc, char **argv)
{
    pthread_t tid;
    int ret;
    char buf[1000];

    sem_init(&g_sem, 0, 0);

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
        pthread_mutex_unlock(&g_tMutex);

        /* Notify the recieve thread*/
        sem_post(&g_sem);
    }
    return 0;

}



