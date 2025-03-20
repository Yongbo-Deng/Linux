#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

/*
socket
bind
listen
accpet
send & recieve
*/

#define ServerPort  8888
#define BackLog     10

int main(int argc, char **argv)
{
    int iSocketServer;
    struct sockaddr_in tSocketServerAddr;
    struct sockaddr_in tSocketClientAddr;
    int iSocketClient;
    int iRet;
    int iAddrLen;
    int iRecvLen;

    unsigned char ucRecvBuf[1000];

    int iClientNum = -1;

    signal(SIGCHLD, SIG_IGN);

    iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (iSocketServer == -1)
    {
        printf("Socket error\n");
        return -1;
    }

    tSocketServerAddr.sin_family = AF_INET;
    tSocketServerAddr.sin_port = htons(ServerPort);     // Host to net, short.
    tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (iRet == -1)
    {
        printf("Bind error\n");
        return -1;
    }

    iRet = listen(iSocketServer, BackLog);
    if (iRet == -1)
    {
        printf("Listen error\n");
        return -1;
    }
    
    while(1)
    {
        iAddrLen = sizeof(struct sockaddr);
        iSocketClient = accept(iSocketServer, (struct sockaddr *) &tSocketClientAddr, &iAddrLen);
        if (iSocketClient != -1)
        {
            iClientNum++;
            printf("Get connection from %s\n", inet_ntoa(tSocketClientAddr.sin_addr));
            
            if (!fork())
            {
                /* On success, the PID of the child process is returned in the parent, and 0 is returned in the child.
                *  On failure, -1 is returned in the parent .
                *  Souce code of the child process:
                */
                while (1) {
                    /* Recieve message from the client and display. */
                    iRecvLen = recv(iSocketClient, ucRecvBuf, 999, 0);
                    if (iRecvLen <= 0)
                    {
                        close(iSocketClient);
                        return -1;
                    }
                    else
                    {
                        ucRecvBuf[iRecvLen] = '\0';
                        printf("Recieve message from Client %d: %s\n", iClientNum, ucRecvBuf);
                    }
                }
            }
            // else {
            //     /* Code of the parent process. */

            // }
        }
    }
    close(iSocketServer);
    close(iSocketClient);
    return 0;
}