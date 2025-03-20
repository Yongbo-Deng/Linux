#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

/* Use semaphore to sync between processes.*/

static char g_buf[1000];
static int g_hasData = 0;
static sem_t g_sem;
static void *my_thread_func(void *data)
{
    while (1) {
        // sleep(1);
        /* Wait for notification and print. */
        sem_wait(&g_sem);
        printf("recv: %s\n", g_buf);
    }
}


int main(int argc, char **argv)
{
    pthread_t tid;
    int ret;

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
        fgets(g_buf, 1000, stdin);

        /* Notify the recieve thread*/
        sem_post(&g_sem);
    }
    return 0;

}



