#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void my_sig_function(int signo)
{
    printf("Signal %d received\n", signo);
}

int main()
{
    int i = 0;

    signal(SIGIO, my_sig_function);

    while (1)
    {
        printf("Waiting for signal %d\n", i++);
        sleep(2);
    }

    return 0;
}