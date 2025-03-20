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
recvfrom
*/

#define ServerPort  8888


int main(int argc, char **argv)
{
    int iSocketServer;
    int iSocketClient;
    struct sockaddr_in tSocketServerAddr;
    struct sockaddr_in tSocketClientAddr;
    int iRet;
    int iAddrLen;
    int iRecvLen;

    unsigned char ucRecvBuf[1000];

    int iClientNum = -1;

    iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocketServer == -1)
    {
        printf("Socket error\n");
        return -1;
    }

    tSocketServerAddr.sin_family        = AF_INET;
    tSocketServerAddr.sin_port          = htons(ServerPort);     // Host to net, short.
    tSocketServerAddr.sin_addr.s_addr   = INADDR_ANY;
    memset(tSocketServerAddr.sin_zero, 0, 8);

    iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
    if (iRet == -1)
    {
        printf("Bind error\n");
        return -1;
    }
    
    while(1)
    {
        iAddrLen = sizeof(struct sockaddr);
        iRecvLen = recvfrom(iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
        if (iRecvLen > 0) {
            ucRecvBuf[iRecvLen] = '\0';
            printf("Get message from client %s: %s", inet_ntoa(tSocketClientAddr.sin_addr), ucRecvBuf);
        }
    }

    close(iSocketServer);
    return 0;
}